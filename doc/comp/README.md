# Sprokit Compiler

## Class Structure
```mermaid
classDiagram
	ASTNode --> ASTNode
	<<abstract>> ASTNode

	Visitor ..> ASTNode
	<<abstract>> Visitor
	Visitor <|-- PrintAST
	PrintAST ..> ASTNode
	Visitor <|-- CollectSymbols
	Visitor <|-- DimensionalAnalysis

	CollectSymbols ..> SymbolTable
	<<abstract>> SymbolTable
	SymbolTable <|-- GlobalSymbols
	SymbolTable <|-- FunctionSymbols
	SymbolTable <|-- TypeSymbols
	ASTNode ..> SymbolTable
```

### Abstract Syntax Tree
```mermaid
classDiagram
	ASTNode --> ASTNode
	<<abstract>> ASTNode
	ASTNode <|-- List
	List --> List
	ASTNode --> SymbolTable
	ASTNode <|-- FunctionDeclaration
	ASTNode <|-- TypeDeclaration
	ASTNode <|-- UnitDeclaration
	ASTNode <|-- VariableDeclaration
	ASTNode <|-- Expression
	<<abstract>> Expression
	Expression <|-- IntegerLiteral
	Expression <|-- BinaryOperator
	Expression <|-- Variable
	<<abstract>> BinaryOperator
	BinaryOperator <|-- Addition
	BinaryOperator <|-- Division
	BinaryOperator <|-- Exponent
	BinaryOperator <|-- Modulo
	BinaryOperator <|-- Multiplication
	BinaryOperator <|-- Subtraction

```

### Dimensional Analysis
```mermaid
classDiagram
	Visitor ..> ASTNode
	<<abstract>> Visitor
	Visitor <|-- DimensionalAnalysis
	DimensionalAnalysis --> UnitParser
	UnitParser ..> UnitScanner
	DimensionalAnalysis ..> Dimensions
	UnitParser ..> Dimensions
```

## Finite State Machines
The json files in this directory are FSM exported from https://www.cs.unc.edu/~otternes/comp455/fsm_designer/.