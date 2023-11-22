; ModuleID = '/OOBChecker/test/test04.c'
source_filename = "/OOBChecker/test/test04.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @f() #0 !dbg !7 {
entry:
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %z = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %x, metadata !11, metadata !DIExpression()), !dbg !13
  store i32 0, i32* %x, align 4, !dbg !13
  call void @llvm.dbg.declare(metadata i32* %y, metadata !14, metadata !DIExpression()), !dbg !15
  store i32 2, i32* %y, align 4, !dbg !15
  call void @llvm.dbg.declare(metadata i32* %z, metadata !16, metadata !DIExpression()), !dbg !17
  %0 = load i32, i32* %x, align 4, !dbg !18
  %cmp = icmp slt i32 %0, 1, !dbg !20
  br i1 %cmp, label %if.then, label %if.end, !dbg !21

if.then:                                          ; preds = %entry
  %1 = load i32, i32* %y, align 4, !dbg !22
  %2 = load i32, i32* %x, align 4, !dbg !24
  %div = sdiv i32 %1, %2, !dbg !25
  store i32 %div, i32* %z, align 4, !dbg !26
  br label %if.end, !dbg !27

if.end:                                           ; preds = %if.then, %entry
  ret void, !dbg !28
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5}
!llvm.ident = !{!6}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 8.0.1- (branches/release_80)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, nameTableKind: None)
!1 = !DIFile(filename: "/OOBChecker/test/test04.c", directory: "/OOBChecker")
!2 = !{}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{!"clang version 8.0.1- (branches/release_80)"}
!7 = distinct !DISubprogram(name: "f", scope: !8, file: !8, line: 1, type: !9, scopeLine: 1, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!8 = !DIFile(filename: "test/test04.c", directory: "/OOBChecker")
!9 = !DISubroutineType(types: !10)
!10 = !{null}
!11 = !DILocalVariable(name: "x", scope: !7, file: !8, line: 2, type: !12)
!12 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!13 = !DILocation(line: 2, column: 7, scope: !7)
!14 = !DILocalVariable(name: "y", scope: !7, file: !8, line: 3, type: !12)
!15 = !DILocation(line: 3, column: 7, scope: !7)
!16 = !DILocalVariable(name: "z", scope: !7, file: !8, line: 4, type: !12)
!17 = !DILocation(line: 4, column: 7, scope: !7)
!18 = !DILocation(line: 5, column: 7, scope: !19)
!19 = distinct !DILexicalBlock(scope: !7, file: !8, line: 5, column: 7)
!20 = !DILocation(line: 5, column: 9, scope: !19)
!21 = !DILocation(line: 5, column: 7, scope: !7)
!22 = !DILocation(line: 6, column: 9, scope: !23)
!23 = distinct !DILexicalBlock(scope: !19, file: !8, line: 5, column: 14)
!24 = !DILocation(line: 6, column: 13, scope: !23)
!25 = !DILocation(line: 6, column: 11, scope: !23)
!26 = !DILocation(line: 6, column: 7, scope: !23)
!27 = !DILocation(line: 7, column: 3, scope: !23)
!28 = !DILocation(line: 8, column: 1, scope: !7)
