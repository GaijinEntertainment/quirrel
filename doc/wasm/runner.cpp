// The VM behind the "Run this code" button on the reference site.
//
// One export, quirrel_eval(source, timeout_msec), which runs a sample the way the
// CI test runner does.
//
// Output is not collected here. print goes to stdout and error to stderr, exactly
// as in sq.cpp, and emscripten hands both to the caller's print callbacks. Going
// through the C streams is what makes io.stdout and io.stderr visible: those write
// to the stream directly and never reach a print function set with sq_setprintfunc.
//
// The sandbox is the module system itself: a file access that serves only the
// sample means require("math") works and require("./other.nut") fails with a
// readable message, without any path filtering of our own.

#include <squirrel.h>
#include <sqstdaux.h>
#include <sqstdsystem.h>
#include <sqasync.h>
#include <sqmodules.h>

#include <emscripten.h>

#include <cstdarg>
#include <cstdio>

// The name a diagnostic shows for the sample. Compiler errors and call stacks quote
// it, so keep it something a reader recognizes as their own code.
static const char *MAIN_SCRIPT = "sample.nut";

static double g_deadline_msec = 0;


static void print_func(HSQUIRRELVM, const char *fmt, ...)
{
    va_list vl;
    va_start(vl, fmt);
    vfprintf(stdout, fmt, vl);
    va_end(vl);
}

static void error_func(HSQUIRRELVM, const char *fmt, ...)
{
    va_list vl;
    va_start(vl, fmt);
    vfprintf(stderr, fmt, vl);
    va_end(vl);
}


// Deliberately not the stock hook from sqstddebug: that one reads its budget from
// the shared state, which debug.set_script_watchdog_timeout_msec can raise from
// inside the sample. The deadline here is the caller's and a script cannot move it.
static bool watchdog(HSQUIRRELVM, bool kick)
{
    if (kick)
        return true;
    return emscripten_get_now() < g_deadline_msec;
}


// Serves the sample and nothing else. Native modules never reach this: they are
// resolved from the module table before any file lookup happens.
struct SampleFileAccess : public ISqModulesFileAccess
{
    string source;

    void destroy() override {}

    void resolveFileName(const char *requested_fn, const char *, string &res) override { res = requested_fn; }

    bool readFile(const string &resolved_fn, const char *requested_fn, vector<char> &buf, string &out_err_msg) override
    {
        if (resolved_fn != MAIN_SCRIPT) {
            out_err_msg = string("cannot read '") + requested_fn +
                          "': this sandbox has no file system, only built-in modules such as \"math\" can be imported";
            return false;
        }
        buf.assign(source.begin(), source.end());
        buf.push_back('\0');
        return true;
    }
};


static void drain_async(HSQUIRRELVM v)
{
    // Same cap as sq.cpp: a pair of tasks awaiting each other never goes idle.
    const int maxIterations = 10000;
    int iterations = 0;
    while (sqasync::has_pending(v)) {
        sqasync::pump(v);
        if (++iterations >= maxIterations) {
            error_func(v, "[sqasync] async drain hit max iterations (%d) -- deadlock?\n", maxIterations);
            break;
        }
    }
}


static bool run(const char *source, int timeout_msec)
{
    HSQUIRRELVM v = sq_open(1024);
    sq_setprintfunc(v, print_func, error_func);
    sqstd_seterrorhandlers(v);

    // Before the libraries: sqstd_register_debuglib installs the stock hook only
    // when the slot is still empty, so setting ours first keeps ours.
    sq_set_watchdog_hook(v, watchdog);
    sq_set_watchdog_timeout_msec(v, timeout_msec);
    g_deadline_msec = emscripten_get_now() + timeout_msec;

    SampleFileAccess access;
    access.source = source;

    bool ok = true;
    {
        // Same set as sq.cpp, minus the test natives, so a sample that runs here
        // runs under the CI test runner too.
        SqModules modules(v, &access);
        modules.registerMathLib();
        modules.registerStringLib();
        modules.registerSystemLib();
        modules.registerIoStreamLib();
        modules.registerIoLib();
        modules.registerDateTimeLib();
        modules.registerDebugLib();

        sqasync::bind(v);
        modules.registerAsyncLib();

        // A browser has no command line, and the truth is the shortest one there
        // is: the sample's own name. __argv exists, so a script that reads it gets
        // an array rather than an error.
        char name[] = "sample.nut";
        char *argv[] = {name};
        sqstd_register_command_line_args(v, 1, argv);

        Sqrat::Object exports;
        SqModules::string errMsg;
        if (!modules.requireModule(MAIN_SCRIPT, true, SqModules::__main__, exports, errMsg)) {
            error_func(v, "Error [%s]\n", errMsg.c_str());
            ok = false;
        }

        drain_async(v);
    }

    sq_close(v);
    return ok;
}


extern "C" {

// Returns 0 when the sample ran to the end. Whatever it printed has already gone to
// the caller through the print callbacks; the flush is what guarantees a sample
// whose last line has no newline is not still sitting in the stream buffer when
// this returns.
EMSCRIPTEN_KEEPALIVE int quirrel_eval(const char *source, int timeout_msec)
{
    // Same reason as in sq.cpp: buffering would let a run of prints overtake an
    // error written between them, and the interleaving of the two streams is
    // exactly what the committed .out files record.
    static bool unbuffered = false;
    if (!unbuffered) {
        setvbuf(stdout, NULL, _IONBF, 0);
        setvbuf(stderr, NULL, _IONBF, 0);
        unbuffered = true;
    }

    const bool ok = run(source, timeout_msec);
    fflush(stdout);
    fflush(stderr);
    return ok ? 0 : 1;
}

EMSCRIPTEN_KEEPALIVE const char *quirrel_version()
{
    return SQUIRREL_VERSION;
}

}
