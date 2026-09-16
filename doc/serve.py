"""Serves the built site for a local look.

    python doc/serve.py [--port 8000] [--dir doc/_site] [--cache-seconds 120]

`python -m http.server` is not enough on Windows: it takes MIME types from the
registry, where .js is often text/plain, and a browser refuses to run a worker
script served as that. The Run button then reports no VM on a site that is fine.
This sets the types that matter and states a cache policy, which the stock server
does not: without one a browser applies its own heuristic and can reuse a
stylesheet for hours, so editing the CSS appears to do nothing.
"""

import argparse
import functools
import http.server
import os

HERE = os.path.dirname(os.path.abspath(__file__))


class Handler(http.server.SimpleHTTPRequestHandler):
    extensions_map = {
        **http.server.SimpleHTTPRequestHandler.extensions_map,
        ".js": "text/javascript",
        ".wasm": "application/wasm",
        ".json": "application/json",
    }

    cache_seconds = 120

    def end_headers(self):
        if self.cache_seconds > 0:
            # The browser may reuse a file for this long without asking. Past it,
            # must-revalidate forbids serving a stale copy while it checks.
            policy = f"max-age={self.cache_seconds}, must-revalidate"
        else:
            policy = "no-store, must-revalidate"
        self.send_header("Cache-Control", policy)
        super().end_headers()

    def send_head(self):
        # SimpleHTTPRequestHandler answers 304 from If-Modified-Since on its own,
        # comparing whole seconds. Two builds inside the same second leave the
        # timestamp unchanged, so that 304 can hand back the older file. Dropping
        # the header makes every revalidation return the current bytes.
        del self.headers["If-Modified-Since"]
        return super().send_head()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--port", type=int, default=8000)
    parser.add_argument("--dir", default=os.path.join(HERE, "_site"))
    parser.add_argument("--cache-seconds", type=int, default=120,
                        help="how long a browser may reuse a file without asking; "
                             "0 disables caching entirely (default: 120)")
    args = parser.parse_args()

    handler = functools.partial(Handler, directory=args.dir)
    Handler.cache_seconds = args.cache_seconds
    print(f"http://localhost:{args.port}/  serving {args.dir}")
    print(f"cache: {args.cache_seconds}s" if args.cache_seconds > 0 else "cache: disabled")
    http.server.ThreadingHTTPServer(("", args.port), handler).serve_forever()


if __name__ == "__main__":
    main()
