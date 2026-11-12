class Foo {
    function DoSomething() {
        println("I'm the base")
    }
    function DoIt() {
        this.DoSomething()
    }
}

class SuperFoo(Foo) {
    //overridden method
    function DoSomething() {
        println("I'm the derived")
    }
    function DoIt() {
        Foo.DoIt.call(this)
    }
}

//creates a new instance of SuperFoo
let inst = SuperFoo()

//prints "I'm the derived"
inst.DoIt()
