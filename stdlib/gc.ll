; ModuleID = 'gc.cpp'
source_filename = "gc.cpp"
target datalayout = "e-m:x-p:32:32-i64:64-f80:32-n8:16:32-a:0:32-S32"
target triple = "i686-pc-windows-msvc19.15.26732"

%struct.GcObj = type { %struct.GcObj*, i8, i8* }

$"\01??_C@_0M@EKOMOFDN@list?5start?6?$AA@" = comdat any

$"\01??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@" = comdat any

$"\01??_C@_0BB@BOEFFFLO@is_reachable?$DN?$CFd?6?$AA@" = comdat any

$"\01??_C@_0N@KCHENNKK@typename?$DN?$CFs?6?$AA@" = comdat any

$"\01??_C@_09CFCANAIO@list?5end?6?$AA@" = comdat any

@"\01?gc_obj_list@@3PAUGcObj@@A" = global %struct.GcObj* null, align 4
@"\01??_C@_0M@EKOMOFDN@list?5start?6?$AA@" = linkonce_odr unnamed_addr constant [12 x i8] c"list start\0A\00", comdat, align 1
@"\01??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@" = linkonce_odr unnamed_addr constant [8 x i8] c"obj=%d\0A\00", comdat, align 1
@"\01??_C@_0BB@BOEFFFLO@is_reachable?$DN?$CFd?6?$AA@" = linkonce_odr unnamed_addr constant [17 x i8] c"is_reachable=%d\0A\00", comdat, align 1
@"\01??_C@_0N@KCHENNKK@typename?$DN?$CFs?6?$AA@" = linkonce_odr unnamed_addr constant [13 x i8] c"typename=%s\0A\00", comdat, align 1
@"\01??_C@_09CFCANAIO@list?5end?6?$AA@" = linkonce_odr unnamed_addr constant [10 x i8] c"list end\0A\00", comdat, align 1

; Function Attrs: noinline optnone
define void @"\01?PrintList@@YAXPAUGcObj@@@Z"(%struct.GcObj*) #0 {
  %2 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %0, %struct.GcObj** %2, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @"\01??_C@_0M@EKOMOFDN@list?5start?6?$AA@", i32 0, i32 0))
  br label %4

; <label>:4:                                      ; preds = %7, %1
  %5 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %6 = icmp ne %struct.GcObj* %5, null
  br i1 %6, label %7, label %24

; <label>:7:                                      ; preds = %4
  %8 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %9 = ptrtoint %struct.GcObj* %8 to i32
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"\01??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@", i32 0, i32 0), i32 %9)
  %11 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %12 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %11, i32 0, i32 1
  %13 = load i8, i8* %12, align 4
  %14 = trunc i8 %13 to i1
  %15 = zext i1 %14 to i32
  %16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @"\01??_C@_0BB@BOEFFFLO@is_reachable?$DN?$CFd?6?$AA@", i32 0, i32 0), i32 %15)
  %17 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %18 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %17, i32 0, i32 2
  %19 = load i8*, i8** %18, align 4
  %20 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @"\01??_C@_0N@KCHENNKK@typename?$DN?$CFs?6?$AA@", i32 0, i32 0), i8* %19)
  %21 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %22 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %21, i32 0, i32 0
  %23 = load %struct.GcObj*, %struct.GcObj** %22, align 4
  store %struct.GcObj* %23, %struct.GcObj** %2, align 4
  br label %4

; <label>:24:                                     ; preds = %4
  %25 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @"\01??_C@_09CFCANAIO@list?5end?6?$AA@", i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...) #1

; Function Attrs: noinline optnone
define i8* @gc_malloc(i32) #0 {
  %2 = alloca i32, align 4
  %3 = alloca %struct.GcObj*, align 4
  %4 = alloca %struct.GcObj*, align 4
  store i32 %0, i32* %2, align 4
  %5 = load %struct.GcObj*, %struct.GcObj** @"\01?gc_obj_list@@3PAUGcObj@@A", align 4
  call void @"\01?PrintList@@YAXPAUGcObj@@@Z"(%struct.GcObj* %5)
  %6 = load %struct.GcObj*, %struct.GcObj** @"\01?gc_obj_list@@3PAUGcObj@@A", align 4
  store %struct.GcObj* %6, %struct.GcObj** %3, align 4
  %7 = load i32, i32* %2, align 4
  %8 = call i8* @malloc(i32 %7)
  %9 = bitcast i8* %8 to %struct.GcObj*
  store %struct.GcObj* %9, %struct.GcObj** %4, align 4
  %10 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %11 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  %12 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %11, i32 0, i32 0
  store %struct.GcObj* %10, %struct.GcObj** %12, align 4
  %13 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  %14 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %13, i32 0, i32 1
  store i8 0, i8* %14, align 4
  %15 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  store %struct.GcObj* %15, %struct.GcObj** @"\01?gc_obj_list@@3PAUGcObj@@A", align 4
  %16 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  %17 = bitcast %struct.GcObj* %16 to i8*
  ret i8* %17
}

declare i8* @malloc(i32) #1

attributes #0 = { noinline optnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"NumRegisterParameters", i32 0}
!1 = !{i32 1, !"wchar_size", i32 2}
!2 = !{!"clang version 6.0.0 (tags/RELEASE_600/final)"}
