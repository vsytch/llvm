; RUN: llc -mtriple wasm32-unknown-unknown-wasm -filetype=obj %s -o - | obj2yaml | FileCheck %s

; foo_alias() function is weak alias of function foo()
; Generates two exports of the same function, one of them weak

<<<<<<< HEAD
@foo_alias = weak hidden alias i32 (...), bitcast (i32 ()* @foo to i32 (...)*)
=======
@foo_alias = weak hidden alias i32 (), i32 ()* @foo

define hidden i32 @call_alias() #0 {
entry:
  %call = call i32 @foo_alias()
  ret i32 %call
}
>>>>>>> 088a118f83a6aef379d0de80ceb9aa764854b9d0

define hidden i32 @foo() #0 {
entry:
  ret i32 0
}

<<<<<<< HEAD
; CHECK:        - Type:            EXPORT
; CHECK-NEXT:     Exports:         
; CHECK-NEXT:       - Name:            foo
; CHECK-NEXT:         Kind:            FUNCTION
; CHECK-NEXT:         Index:           0
; CHECK-NEXT:       - Name:            foo_alias
; CHECK-NEXT:         Kind:            FUNCTION
; CHECK-NEXT:         Index:           0

=======

; CHECK:        - Type:            TYPE
; CHECK-NEXT:     Signatures:      
; CHECK-NEXT:       - Index:           0
; CHECK-NEXT:         ReturnType:      I32
; CHECK-NEXT:         ParamTypes:      

; CHECK:        - Type:            IMPORT
; CHECK-NEXT:     Imports:         
; CHECK-NEXT:       - Module:          env
; CHECK-NEXT:         Field:           foo_alias
; CHECK-NEXT:         Kind:            FUNCTION
; CHECK-NEXT:         SigIndex:        0

; CHECK:        - Type:            FUNCTION
; CHECK-NEXT:     FunctionTypes:   [ 0, 0 ]

; CHECK:        - Type:            EXPORT
; CHECK-NEXT:     Exports:         
; CHECK-NEXT:       - Name:            call_alias
; CHECK-NEXT:         Kind:            FUNCTION
; CHECK-NEXT:         Index:           1
; CHECK-NEXT:       - Name:            foo
; CHECK-NEXT:         Kind:            FUNCTION
; CHECK-NEXT:         Index:           2
; CHECK-NEXT:       - Name:            foo_alias
; CHECK-NEXT:         Kind:            FUNCTION
; CHECK-NEXT:         Index:           2
>>>>>>> 088a118f83a6aef379d0de80ceb9aa764854b9d0

; CHECK:        - Type:            CUSTOM
; CHECK-NEXT:     Name:            name
; CHECK-NEXT:     FunctionNames:   
; CHECK-NEXT:       - Index:           0
<<<<<<< HEAD
=======
; CHECK-NEXT:         Name:            foo_alias
; CHECK-NEXT:       - Index:           1
; CHECK-NEXT:         Name:            call_alias
; CHECK-NEXT:       - Index:           2
>>>>>>> 088a118f83a6aef379d0de80ceb9aa764854b9d0
; CHECK-NEXT:         Name:            foo
; CHECK-NEXT:   - Type:            CUSTOM
; CHECK-NEXT:     Name:            linking
; CHECK-NEXT:     DataSize:        0
; CHECK-NEXT:     DataAlignment:   0
; CHECK-NEXT:     SymbolInfo:      
; CHECK-NEXT:       - Name:            foo_alias
; CHECK-NEXT:         Flags:           1
; CHECK-NEXT: ...
