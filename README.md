# Avalon - a classical and quantum programming language

Quantum computing is advancing faster and faster than expected.  
Avalon is a new programming language and my goal in creating it is to have a programming language that works on classical as well quantum computers.  
The reason for targeting both forms of computations is due to the immediate need to be able to test classical as well as quantum algorithms in the same language.

## What (will) does it look like?

Here is what an Avalon module looks like.
```
-- A namespace declaration
namespace io -(
    -- A global public constant variable that contains a named tuple
    public val settings = (
        name    = "Input/Output standard library",
        version = "0.0.0"
    )
    
    -- The classical `println` function but it doesn't do anything at the moment
    -- Against better judgement, all declarations are public by default hence the lack of the `public` keyword as in the variable declaration above
    def println = (val str : string) -> int:
        return 0
)-

```

And here is how we would use the module above.  
Note the import declaration. The imported name is the fully qualified name that resolves to the file path that contains the declarations.  
The import makes available all public declarations from the imported file in the current file.  
Also note the absence of a namespace declaration. Just like in C++, when we want want declarations in the global namespace, we don't specify a namespace (or specify a namespace without a name.
```
-- An import declaration
import io

-- A type declaration - the maybe type
type maybe = (a):
    None
    | Just(a)

-- The main function - the entry point
def __main__ = (val args : [string]) -> void:
    var message = "Hello World"
    io.println(message)

```

So basically the code looks like Python with a few differences here and there.  
More examples can be found in the `tests` folder.

## State of the project

Right now, I'm working on semantic analysis.  
After which I will focus on code generation for both the classical and quantum parts.  
On the classical side, the goal is to transpile to C code.  
On the quantum side, we will generate QASM code for the IBM quantum computer.

Here is what's be done and what's on the way topdown.

* `Lexer` : done.
* `Parser` : done.
* `Import module` : done.
* `Semantic analyzer and type inference engine`:
    - `Type declarations`: done.
    - `Function declarations`: done.
    - `Variable declarations`: done.
    - `Statement declarations`:
        - `While statements`: **not** done.
        - `If statements`: **not** done.
        - `Expression statements`:
            - `Literal expressionss`: done.
            - `Container expressions`: done.
            - `Constructor initializers`: done.
            - `Function calls`: done.
            - `Variable expressions`: done.
            - `Grouped expressions`: done.
            - `Cast expressions`: done.
            - `Unary expressions`: done.
            - `Binary expressions`: done.
            - `Match expressions`: done.
            - `Assignment expressions`: *working on it*.
* `Code generation`: **not** done.

**NOTE**: I have not included optimization right now as my priority is to get things that work first. Code optimization will come (dead) last.

## Installation

Download the code in this repository and make sure to have the latest versions of GMP and BOOST installed.  
Then run the makefile.  
```shell
$ make clean && make
```

## Contributing

Thank you for considering it.  
This is actually my first major C++ project. I chose it because it simply offered more than C does even though it is the language I am most comfortable with.  
So you will find bugs left and right. Violated idioms and the Lord knows what else. Some of those I am aware of but not motivated to fix because they are convinient to have (such as the abuse of shared pointers and their use - e.g. passing them to functions without using const).  
If you have improvements to suggest, shoot me an email.  
I am not currently accepting pull requests until I have a fully working compiler.

## Contact

You can reach me at <a href="mailto:nbashige@gmail.com">nbashige@gmail.com</a>

## License

This code is licensed under the MIT license. Please see the LICENSE file for the terms.

## Copyrights

I, Ntwali Bashige Toussaint, retain all copyrights in and of the entirety of the work (source code and other files) in this repository whether explicitly mentionned or not in any of the files in this project.

In case you found code that is yours and I failed to give attribution or I am claiming copyright of the code, please do email to resolve the issue.

