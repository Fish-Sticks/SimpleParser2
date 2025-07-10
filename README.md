# SimpleParser2
SimpleParser2 is an example of LL(1) parsing techniques such as pratt and tail recursive descent. The pratt implementation has postfix notation for numbers such as 1M = (1 million), 1K = (1 thousand), etc. The tail recursive implementation doesn't have this behavior and it's left as an exercise to the reader.  
<br>
This project is an example of how to create a parser from scratch. It includes a calculator which has an obfuscator, simplifier, evaluator, and expression parser to AST.  
<br>
The project contains programming examples such as AST (abstract syntax trees), visitor pattern, parsing techniques, and how a calculator can be made by merging all the concepts.  
<br>
The project has four different visitors used. They can be found in /Parser/AST/VisitorImpls.hpp. 
<br>
The **EvaluationVisitor** class contains a visitor which shows how to evaluate the AST nodes using a number stack.  
<br>
The **DisplayVisitor** class contains a visitor which parenthesizes every single node in the AST and displays it, giving you exact steps to order of operations that the parser made. This is like a view into the AST tree, as it directly shows what it holds.  
<br>
The **SmartDisplayVisitor** class contains a visitor which only parenthesizes the needed operations. For example **5 + 3 * 10** and **5 + (3 * 10)** are the same thing to it, and it will output **5 + 3 * 10**. The AST holds no parenthesized information, as the smart display can identify where parenthesizes must reside for the mathematical expression to match its AST equivalent. This is useful for taking over parenthesized outputs and simplifying them into only needed parenthesis.  
<br>
The **ExplodeVisitor** class contains a unique routine which will "obfuscate" your mathematical expression. It takes the AST and returns a clone of the AST with new logic inside it. It precomputes values using random, and forces the expressions to match their original number, but not directly state it. By doing multiple passes of this, you end up with an obfuscated sequence of numbers.  
Warning: The ExplodeVisitor can be imprecise because of floating point values. It is recommended to use fractions to express decimals if you will actually use this.
<br>
If you would like to help contribute, or add anything to the fun project, just make a pull request and/or message me.
