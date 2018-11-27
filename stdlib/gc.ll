; ModuleID = 'gc.cpp'
source_filename = "gc.cpp"
target datalayout = "e-m:x-p:32:32-i64:64-f80:32-n8:16:32-a:0:32-S32"
target triple = "i686-pc-windows-msvc19.15.26732"

%struct.GcObj = type { %struct.GcObj*, %struct.GcObj*, i8, i8* }

$"\01??_C@_0M@EKOMOFDN@list?5start?6?$AA@" = comdat any

$"\01??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@" = comdat any

$"\01??_C@_0BB@BOEFFFLO@is_reachable?$DN?$CFd?6?$AA@" = comdat any

$"\01??_C@_0N@KCHENNKK@typename?$DN?$CFs?6?$AA@" = comdat any

$"\01??_C@_0L@PAMKGGHN@list?5end?6?6?$AA@" = comdat any

@"\01?gc_obj_list@@3PAUGcObj@@A" = global %struct.GcObj* null, align 4
@"\01?gc_root_list@@3PAUGcObj@@A" = global %struct.GcObj* null, align 4
@"\01??_C@_0M@EKOMOFDN@list?5start?6?$AA@" = linkonce_odr unnamed_addr constant [12 x i8] c"list start\0A\00", comdat, align 1
@"\01??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@" = linkonce_odr unnamed_addr constant [8 x i8] c"obj=%d\0A\00", comdat, align 1
@"\01??_C@_0BB@BOEFFFLO@is_reachable?$DN?$CFd?6?$AA@" = linkonce_odr unnamed_addr constant [17 x i8] c"is_reachable=%d\0A\00", comdat, align 1
@"\01??_C@_0N@KCHENNKK@typename?$DN?$CFs?6?$AA@" = linkonce_odr unnamed_addr constant [13 x i8] c"typename=%s\0A\00", comdat, align 1
@"\01??_C@_0L@PAMKGGHN@list?5end?6?6?$AA@" = linkonce_odr unnamed_addr constant [11 x i8] c"list end\0A\0A\00", comdat, align 1

; Function Attrs: noinline optnone
define void @"\01?PrintList@@YAXPAUGcObj@@_N@Z"(%struct.GcObj*, i1 zeroext) #0 {
  %3 = alloca i8, align 1
  %4 = alloca %struct.GcObj*, align 4
  %5 = zext i1 %1 to i8
  store i8 %5, i8* %3, align 1
  store %struct.GcObj* %0, %struct.GcObj** %4, align 4
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @"\01??_C@_0M@EKOMOFDN@list?5start?6?$AA@", i32 0, i32 0))
  br label %7

; <label>:7:                                      ; preds = %34, %2
  %8 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  %9 = icmp ne %struct.GcObj* %8, null
  br i1 %9, label %10, label %35

; <label>:10:                                     ; preds = %7
  %11 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  %12 = ptrtoint %struct.GcObj* %11 to i32
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"\01??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@", i32 0, i32 0), i32 %12)
  %14 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  %15 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %14, i32 0, i32 2
  %16 = load i8, i8* %15, align 4
  %17 = trunc i8 %16 to i1
  %18 = zext i1 %17 to i32
  %19 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @"\01??_C@_0BB@BOEFFFLO@is_reachable?$DN?$CFd?6?$AA@", i32 0, i32 0), i32 %18)
  %20 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  %21 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %20, i32 0, i32 3
  %22 = load i8*, i8** %21, align 4
  %23 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @"\01??_C@_0N@KCHENNKK@typename?$DN?$CFs?6?$AA@", i32 0, i32 0), i8* %22)
  %24 = load i8, i8* %3, align 1
  %25 = trunc i8 %24 to i1
  br i1 %25, label %26, label %30

; <label>:26:                                     ; preds = %10
  %27 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  %28 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %27, i32 0, i32 1
  %29 = load %struct.GcObj*, %struct.GcObj** %28, align 4
  store %struct.GcObj* %29, %struct.GcObj** %4, align 4
  br label %34

; <label>:30:                                     ; preds = %10
  %31 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  %32 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %31, i32 0, i32 0
  %33 = load %struct.GcObj*, %struct.GcObj** %32, align 4
  store %struct.GcObj* %33, %struct.GcObj** %4, align 4
  br label %34

; <label>:34:                                     ; preds = %30, %26
  br label %7

; <label>:35:                                     ; preds = %7
  %36 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @"\01??_C@_0L@PAMKGGHN@list?5end?6?6?$AA@", i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...) #1

; Function Attrs: noinline optnone
define i8* @gc_malloc(i32) #0 {
  %2 = alloca i32, align 4
  %3 = alloca %struct.GcObj*, align 4
  store i32 %0, i32* %2, align 4
  %4 = load %struct.GcObj*, %struct.GcObj** @"\01?gc_obj_list@@3PAUGcObj@@A", align 4
  call void @"\01?PrintList@@YAXPAUGcObj@@_N@Z"(%struct.GcObj* %4, i1 zeroext true)
  %5 = load i32, i32* %2, align 4
  %6 = call i8* @malloc(i32 %5)
  %7 = bitcast i8* %6 to %struct.GcObj*
  store %struct.GcObj* %7, %struct.GcObj** %3, align 4
  %8 = load %struct.GcObj*, %struct.GcObj** @"\01?gc_obj_list@@3PAUGcObj@@A", align 4
  %9 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %10 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %9, i32 0, i32 0
  store %struct.GcObj* %8, %struct.GcObj** %10, align 4
  %11 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %12 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %11, i32 0, i32 1
  store %struct.GcObj* null, %struct.GcObj** %12, align 4
  %13 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %14 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %13, i32 0, i32 2
  store i8 0, i8* %14, align 4
  %15 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  store %struct.GcObj* %15, %struct.GcObj** @"\01?gc_obj_list@@3PAUGcObj@@A", align 4
  %16 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %17 = bitcast %struct.GcObj* %16 to i8*
  ret i8* %17
}

declare i8* @malloc(i32) #1

; Function Attrs: noinline optnone
define void @gc_add_root(%struct.GcObj*) #0 {
  %2 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %0, %struct.GcObj** %2, align 4
  %3 = load %struct.GcObj*, %struct.GcObj** @"\01?gc_root_list@@3PAUGcObj@@A", align 4
  call void @"\01?PrintList@@YAXPAUGcObj@@_N@Z"(%struct.GcObj* %3, i1 zeroext true)
  %4 = load %struct.GcObj*, %struct.GcObj** @"\01?gc_root_list@@3PAUGcObj@@A", align 4
  %5 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %6 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %5, i32 0, i32 1
  store %struct.GcObj* %4, %struct.GcObj** %6, align 4
  %7 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  store %struct.GcObj* %7, %struct.GcObj** @"\01?gc_root_list@@3PAUGcObj@@A", align 4
  ret void
}

attributes #0 = { noinline optnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"NumRegisterParameters", i32 0}
!1 = !{i32 1, !"wchar_size", i32 2}
!2 = !{!"clang version 6.0.0 (tags/RELEASE_600/final)"}
