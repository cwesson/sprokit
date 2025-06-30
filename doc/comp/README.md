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

## Symbol Table
```
var a
var b
func test() {
	var c
	if {
		var i
	} else {
		var j
	}
	var k
	var l
}
```
```mermaid
flowchart LR
	Global --> a --> b --> $func_test --> c --> $if --> i
	c --> $else --> j
	c --> k --> l
```

## Abstract Data Types
```mermaid
classDiagram
	class Type
	<<abstract>> Type
	Type <|-- UnknownType
	Type <-- UserType
	Type <|-- PrimitiveType
	<<abstract>> PrimitiveType
	PrimitiveType <|-- BoolType
	PrimitiveType <|-- IntType
	PrimitiveType <|-- FloatType
```

```mermaid
sequenceDiagram
	Note over Semantics,TypeU: Check if TypeU is<br>convertible to TypeT
	Semantics ->>+ TypeU: convertibleTo(t)
	TypeU ->>+ TypeT: accept(u)
	TypeT ->>+ TypeU: visit(t)
	TypeU -->>- TypeT: true
	TypeT -->>- TypeU: true
	TypeU -->>- Semantics: true
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