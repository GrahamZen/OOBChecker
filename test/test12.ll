; ModuleID = '/OOBChecker/test/test12.c'
source_filename = "/OOBChecker/test/test12.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @f() #0 !dbg !7 {
entry:
  %i = alloca i32, align 4
  %sum = alloca i32, align 4
  %y = alloca i32, align 4
  %z = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %i, metadata !11, metadata !DIExpression()), !dbg !13
  %call = call i32 @getchar(), !dbg !14
  store i32 %call, i32* %i, align 4, !dbg !13
  call void @llvm.dbg.declare(metadata i32* %sum, metadata !15, metadata !DIExpression()), !dbg !16
  store i32 0, i32* %sum, align 4, !dbg !16
  br label %while.cond, !dbg !17

while.cond:                                       ; preds = %while.body, %entry
  %0 = load i32, i32* %sum, align 4, !dbg !18
  %cmp = icmp slt i32 %0, 50, !dbg !19
  br i1 %cmp, label %while.body, label %while.end, !dbg !17

while.body:                                       ; preds = %while.cond
  %1 = load i32, i32* %i, align 4, !dbg !20
  %2 = load i32, i32* %sum, align 4, !dbg !22
  %add = add nsw i32 %2, %1, !dbg !22
  store i32 %add, i32* %sum, align 4, !dbg !22
  br label %while.cond, !dbg !17, !llvm.loop !23

while.end:                                        ; preds = %while.cond
  call void @llvm.dbg.declare(metadata i32* %y, metadata !25, metadata !DIExpression()), !dbg !26
  %3 = load i32, i32* %sum, align 4, !dbg !27
  %sub = sub nsw i32 %3, 55, !dbg !28
  store i32 %sub, i32* %y, align 4, !dbg !26
  call void @llvm.dbg.declare(metadata i32* %z, metadata !29, metadata !DIExpression()), !dbg !30
  %4 = load i32, i32* %i, align 4, !dbg !31
  %5 = load i32, i32* %y, align 4, !dbg !32
  %div = sdiv i32 %4, %5, !dbg !33
  store i32 %div, i32* %z, align 4, !dbg !30
  ret void, !dbg !34
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare dso_local i32 @getchar() #2

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5}
!llvm.ident = !{!6}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 8.0.1- (branches/release_80)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, nameTableKind: None)
!1 = !DIFile(filename: "/OOBChecker/test/test12.c", directory: "/OOBChecker")
!2 = !{}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{!"clang version 8.0.1- (branches/release_80)"}
!7 = distinct !DISubprogram(name: "f", scope: !8, file: !8, line: 3, type: !9, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!8 = !DIFile(filename: "test/test12.c", directory: "/OOBChecker")
!9 = !DISubroutineType(types: !10)
!10 = !{null}
!11 = !DILocalVariable(name: "i", scope: !7, file: !8, line: 4, type: !12)
!12 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!13 = !DILocation(line: 4, column: 7, scope: !7)
!14 = !DILocation(line: 4, column: 11, scope: !7)
!15 = !DILocalVariable(name: "sum", scope: !7, file: !8, line: 5, type: !12)
!16 = !DILocation(line: 5, column: 7, scope: !7)
!17 = !DILocation(line: 6, column: 3, scope: !7)
!18 = !DILocation(line: 6, column: 10, scope: !7)
!19 = !DILocation(line: 6, column: 14, scope: !7)
!20 = !DILocation(line: 7, column: 12, scope: !21)
!21 = distinct !DILexicalBlock(scope: !7, file: !8, line: 6, column: 20)
!22 = !DILocation(line: 7, column: 9, scope: !21)
!23 = distinct !{!23, !17, !24}
!24 = !DILocation(line: 8, column: 3, scope: !7)
!25 = !DILocalVariable(name: "y", scope: !7, file: !8, line: 9, type: !12)
!26 = !DILocation(line: 9, column: 7, scope: !7)
!27 = !DILocation(line: 9, column: 11, scope: !7)
!28 = !DILocation(line: 9, column: 15, scope: !7)
!29 = !DILocalVariable(name: "z", scope: !7, file: !8, line: 10, type: !12)
!30 = !DILocation(line: 10, column: 7, scope: !7)
!31 = !DILocation(line: 10, column: 11, scope: !7)
!32 = !DILocation(line: 10, column: 15, scope: !7)
!33 = !DILocation(line: 10, column: 13, scope: !7)
!34 = !DILocation(line: 11, column: 1, scope: !7)
