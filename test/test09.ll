; ModuleID = '/OOBChecker/test/test09.c'
source_filename = "/OOBChecker/test/test09.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @f() #0 !dbg !7 {
entry:
  %in = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %out = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %in, metadata !11, metadata !DIExpression()), !dbg !13
  %call = call i32 @getchar(), !dbg !14
  store i32 %call, i32* %in, align 4, !dbg !13
  call void @llvm.dbg.declare(metadata i32* %a, metadata !15, metadata !DIExpression()), !dbg !17
  store i32 10, i32* %a, align 4, !dbg !17
  call void @llvm.dbg.declare(metadata i32* %b, metadata !18, metadata !DIExpression()), !dbg !19
  store i32 2, i32* %b, align 4, !dbg !19
  %0 = load i32, i32* %in, align 4, !dbg !20
  %cmp = icmp sgt i32 %0, 0, !dbg !22
  br i1 %cmp, label %if.then, label %if.else, !dbg !23

if.then:                                          ; preds = %entry
  %1 = load i32, i32* %in, align 4, !dbg !24
  %2 = load i32, i32* %b, align 4, !dbg !26
  %add = add i32 %1, %2, !dbg !27
  store i32 %add, i32* %b, align 4, !dbg !28
  br label %if.end4, !dbg !29

if.else:                                          ; preds = %entry
  %3 = load i32, i32* %in, align 4, !dbg !30
  %cmp1 = icmp eq i32 %3, 0, !dbg !32
  br i1 %cmp1, label %if.then2, label %if.else3, !dbg !33

if.then2:                                         ; preds = %if.else
  store i32 0, i32* %b, align 4, !dbg !34
  br label %if.end, !dbg !36

if.else3:                                         ; preds = %if.else
  %4 = load i32, i32* %in, align 4, !dbg !37
  %5 = load i32, i32* %b, align 4, !dbg !39
  %sub = sub i32 %4, %5, !dbg !40
  store i32 %sub, i32* %b, align 4, !dbg !41
  br label %if.end

if.end:                                           ; preds = %if.else3, %if.then2
  br label %if.end4

if.end4:                                          ; preds = %if.end, %if.then
  call void @llvm.dbg.declare(metadata i32* %out, metadata !42, metadata !DIExpression()), !dbg !43
  %6 = load i32, i32* %a, align 4, !dbg !44
  %7 = load i32, i32* %b, align 4, !dbg !45
  %div = udiv i32 %6, %7, !dbg !46
  store i32 %div, i32* %out, align 4, !dbg !43
  ret void, !dbg !47
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
!1 = !DIFile(filename: "/OOBChecker/test/test09.c", directory: "/OOBChecker")
!2 = !{}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{!"clang version 8.0.1- (branches/release_80)"}
!7 = distinct !DISubprogram(name: "f", scope: !8, file: !8, line: 3, type: !9, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!8 = !DIFile(filename: "test/test09.c", directory: "/OOBChecker")
!9 = !DISubroutineType(types: !10)
!10 = !{null}
!11 = !DILocalVariable(name: "in", scope: !7, file: !8, line: 4, type: !12)
!12 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!13 = !DILocation(line: 4, column: 7, scope: !7)
!14 = !DILocation(line: 4, column: 12, scope: !7)
!15 = !DILocalVariable(name: "a", scope: !7, file: !8, line: 5, type: !16)
!16 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!17 = !DILocation(line: 5, column: 16, scope: !7)
!18 = !DILocalVariable(name: "b", scope: !7, file: !8, line: 6, type: !16)
!19 = !DILocation(line: 6, column: 16, scope: !7)
!20 = !DILocation(line: 8, column: 7, scope: !21)
!21 = distinct !DILexicalBlock(scope: !7, file: !8, line: 8, column: 7)
!22 = !DILocation(line: 8, column: 10, scope: !21)
!23 = !DILocation(line: 8, column: 7, scope: !7)
!24 = !DILocation(line: 9, column: 9, scope: !25)
!25 = distinct !DILexicalBlock(scope: !21, file: !8, line: 8, column: 15)
!26 = !DILocation(line: 9, column: 14, scope: !25)
!27 = !DILocation(line: 9, column: 12, scope: !25)
!28 = !DILocation(line: 9, column: 7, scope: !25)
!29 = !DILocation(line: 10, column: 3, scope: !25)
!30 = !DILocation(line: 10, column: 14, scope: !31)
!31 = distinct !DILexicalBlock(scope: !21, file: !8, line: 10, column: 14)
!32 = !DILocation(line: 10, column: 17, scope: !31)
!33 = !DILocation(line: 10, column: 14, scope: !21)
!34 = !DILocation(line: 11, column: 7, scope: !35)
!35 = distinct !DILexicalBlock(scope: !31, file: !8, line: 10, column: 23)
!36 = !DILocation(line: 12, column: 3, scope: !35)
!37 = !DILocation(line: 13, column: 9, scope: !38)
!38 = distinct !DILexicalBlock(scope: !31, file: !8, line: 12, column: 10)
!39 = !DILocation(line: 13, column: 14, scope: !38)
!40 = !DILocation(line: 13, column: 12, scope: !38)
!41 = !DILocation(line: 13, column: 7, scope: !38)
!42 = !DILocalVariable(name: "out", scope: !7, file: !8, line: 16, type: !12)
!43 = !DILocation(line: 16, column: 7, scope: !7)
!44 = !DILocation(line: 16, column: 13, scope: !7)
!45 = !DILocation(line: 16, column: 17, scope: !7)
!46 = !DILocation(line: 16, column: 15, scope: !7)
!47 = !DILocation(line: 17, column: 1, scope: !7)
