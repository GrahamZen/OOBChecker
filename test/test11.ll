; ModuleID = '/OOBChecker/test/test11.c'
source_filename = "/OOBChecker/test/test11.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @f() #0 !dbg !7 {
entry:
  %i = alloca i32, align 4
  %sum = alloca i32, align 4
  %z = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %i, metadata !11, metadata !DIExpression()), !dbg !13
  call void @llvm.dbg.declare(metadata i32* %sum, metadata !14, metadata !DIExpression()), !dbg !15
  store i32 0, i32* %sum, align 4, !dbg !15
  store i32 0, i32* %i, align 4, !dbg !16
  br label %for.cond, !dbg !18

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, i32* %i, align 4, !dbg !19
  %cmp = icmp slt i32 %0, 10, !dbg !21
  br i1 %cmp, label %for.body, label %for.end, !dbg !22

for.body:                                         ; preds = %for.cond
  %1 = load i32, i32* %sum, align 4, !dbg !23
  %add = add nsw i32 %1, 1, !dbg !23
  store i32 %add, i32* %sum, align 4, !dbg !23
  br label %for.inc, !dbg !25

for.inc:                                          ; preds = %for.body
  %2 = load i32, i32* %i, align 4, !dbg !26
  %inc = add nsw i32 %2, 1, !dbg !26
  store i32 %inc, i32* %i, align 4, !dbg !26
  br label %for.cond, !dbg !27, !llvm.loop !28

for.end:                                          ; preds = %for.cond
  call void @llvm.dbg.declare(metadata i32* %z, metadata !30, metadata !DIExpression()), !dbg !31
  %3 = load i32, i32* %i, align 4, !dbg !32
  %4 = load i32, i32* %sum, align 4, !dbg !33
  %div = sdiv i32 %3, %4, !dbg !34
  store i32 %div, i32* %z, align 4, !dbg !31
  ret void, !dbg !35
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5}
!llvm.ident = !{!6}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 8.0.1- (branches/release_80)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, nameTableKind: None)
!1 = !DIFile(filename: "/OOBChecker/test/test11.c", directory: "/OOBChecker")
!2 = !{}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{!"clang version 8.0.1- (branches/release_80)"}
!7 = distinct !DISubprogram(name: "f", scope: !8, file: !8, line: 1, type: !9, scopeLine: 1, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!8 = !DIFile(filename: "test/test11.c", directory: "/OOBChecker")
!9 = !DISubroutineType(types: !10)
!10 = !{null}
!11 = !DILocalVariable(name: "i", scope: !7, file: !8, line: 2, type: !12)
!12 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!13 = !DILocation(line: 2, column: 7, scope: !7)
!14 = !DILocalVariable(name: "sum", scope: !7, file: !8, line: 3, type: !12)
!15 = !DILocation(line: 3, column: 7, scope: !7)
!16 = !DILocation(line: 4, column: 10, scope: !17)
!17 = distinct !DILexicalBlock(scope: !7, file: !8, line: 4, column: 3)
!18 = !DILocation(line: 4, column: 8, scope: !17)
!19 = !DILocation(line: 4, column: 15, scope: !20)
!20 = distinct !DILexicalBlock(scope: !17, file: !8, line: 4, column: 3)
!21 = !DILocation(line: 4, column: 17, scope: !20)
!22 = !DILocation(line: 4, column: 3, scope: !17)
!23 = !DILocation(line: 5, column: 9, scope: !24)
!24 = distinct !DILexicalBlock(scope: !20, file: !8, line: 4, column: 28)
!25 = !DILocation(line: 6, column: 3, scope: !24)
!26 = !DILocation(line: 4, column: 24, scope: !20)
!27 = !DILocation(line: 4, column: 3, scope: !20)
!28 = distinct !{!28, !22, !29}
!29 = !DILocation(line: 6, column: 3, scope: !17)
!30 = !DILocalVariable(name: "z", scope: !7, file: !8, line: 7, type: !12)
!31 = !DILocation(line: 7, column: 7, scope: !7)
!32 = !DILocation(line: 7, column: 11, scope: !7)
!33 = !DILocation(line: 7, column: 15, scope: !7)
!34 = !DILocation(line: 7, column: 13, scope: !7)
!35 = !DILocation(line: 8, column: 1, scope: !7)
