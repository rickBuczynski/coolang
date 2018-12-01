; ModuleID = '.\gc.cpp'
source_filename = ".\5Cgc.cpp"
target datalayout = "e-m:x-p:32:32-i64:64-f80:32-n8:16:32-a:0:32-S32"
target triple = "i386-pc-windows-msvc19.16.27024"

%class.GcList = type { %struct.GcObj* }
%struct.GcObj = type { %struct.GcObj*, %struct.GcObj*, %struct.GcObj*, %struct.GcObj*, i8, i8* }
%class.GcList.0 = type { %struct.GcObj* }
%struct._iobuf = type { i8* }
%struct.__crt_locale_pointers = type { %struct.__crt_locale_data*, %struct.__crt_multibyte_data* }
%struct.__crt_locale_data = type opaque
%struct.__crt_multibyte_data = type opaque

$"?PrintList@?$GcList@VGcObjList@@@@QAEXXZ" = comdat any

$"?PushFront@?$GcList@VGcObjList@@@@QAEXPAUGcObj@@@Z" = comdat any

$"?PushFront@?$GcList@VGcRootList@@@@QAEXPAUGcObj@@@Z" = comdat any

$"?PrintList@?$GcList@VGcRootList@@@@QAEXXZ" = comdat any

$"?Remove@?$GcList@VGcRootList@@@@QAEXPAUGcObj@@@Z" = comdat any

$fprintf = comdat any

$"?ListName@GcObjList@@SAPBDXZ" = comdat any

$printf = comdat any

$"?GetPrev@GcObjList@@SAPAUGcObj@@PAU2@@Z" = comdat any

$"?GetNext@GcObjList@@SAPAUGcObj@@PAU2@@Z" = comdat any

$_vfprintf_l = comdat any

$__local_stdio_printf_options = comdat any

$"?SetNext@GcObjList@@SAXPAUGcObj@@0@Z" = comdat any

$"?SetPrev@GcObjList@@SAXPAUGcObj@@0@Z" = comdat any

$"?SetNext@GcRootList@@SAXPAUGcObj@@0@Z" = comdat any

$"?SetPrev@GcRootList@@SAXPAUGcObj@@0@Z" = comdat any

$"?ListName@GcRootList@@SAPBDXZ" = comdat any

$"?GetPrev@GcRootList@@SAPAUGcObj@@PAU2@@Z" = comdat any

$"?GetNext@GcRootList@@SAPAUGcObj@@PAU2@@Z" = comdat any

$"??_C@_09EBJFJNG@?$CFs?5start?6?$AA@" = comdat any

$"??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@" = comdat any

$"??_C@_0BD@HGGAKDEI@?5?5is_reachable?$DN?$CFd?6?$AA@" = comdat any

$"??_C@_0P@DPHCNOIJ@?5?5typename?$DN?$CFs?6?$AA@" = comdat any

$"??_C@_04MDPHBIK@obj?6?$AA@" = comdat any

$"??_C@_0CO@HGIHIH@?5?5BADBADBADBADBADBADBADBADBADBAD@" = comdat any

$"??_C@_08BPFNJJP@?$CFs?5end?6?6?$AA@" = comdat any

$"?_OptionsStorage@?1??__local_stdio_printf_options@@9@4_KA" = comdat any

$"??_C@_07DGBOGBKN@gc?5objs?$AA@" = comdat any

$"??_C@_08NJAKNIEC@gc?5roots?$AA@" = comdat any

$"??_C@_0CP@HIJCJBBI@Tried?5to?5remove?5an?5obj?5that?8s?5no@" = comdat any

@"?gc_obj_list@@3V?$GcList@VGcObjList@@@@A" = dso_local global %class.GcList zeroinitializer, align 4
@"?gc_root_list@@3V?$GcList@VGcRootList@@@@A" = dso_local global %class.GcList.0 zeroinitializer, align 4
@"??_C@_09EBJFJNG@?$CFs?5start?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [10 x i8] c"%s start\0A\00", comdat, align 1
@"??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [8 x i8] c"obj=%d\0A\00", comdat, align 1
@"??_C@_0BD@HGGAKDEI@?5?5is_reachable?$DN?$CFd?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [19 x i8] c"  is_reachable=%d\0A\00", comdat, align 1
@"??_C@_0P@DPHCNOIJ@?5?5typename?$DN?$CFs?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [15 x i8] c"  typename=%s\0A\00", comdat, align 1
@"??_C@_04MDPHBIK@obj?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [5 x i8] c"obj\0A\00", comdat, align 1
@"??_C@_0CO@HGIHIH@?5?5BADBADBADBADBADBADBADBADBADBAD@" = linkonce_odr dso_local unnamed_addr constant [46 x i8] c"  BADBADBADBADBADBADBADBADBADBADBADBADBADBAD\0A\00", comdat, align 1
@"??_C@_08BPFNJJP@?$CFs?5end?6?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [9 x i8] c"%s end\0A\0A\00", comdat, align 1
@"?_OptionsStorage@?1??__local_stdio_printf_options@@9@4_KA" = linkonce_odr dso_local global i64 0, comdat, align 8
@"??_C@_07DGBOGBKN@gc?5objs?$AA@" = linkonce_odr dso_local unnamed_addr constant [8 x i8] c"gc objs\00", comdat, align 1
@"??_C@_08NJAKNIEC@gc?5roots?$AA@" = linkonce_odr dso_local unnamed_addr constant [9 x i8] c"gc roots\00", comdat, align 1
@"??_C@_0CP@HIJCJBBI@Tried?5to?5remove?5an?5obj?5that?8s?5no@" = linkonce_odr dso_local unnamed_addr constant [47 x i8] c"Tried to remove an obj that's not in list: %s\0A\00", comdat, align 1

; Function Attrs: noinline optnone
define dso_local void @print_gc_obj_list() #0 {
  call x86_thiscallcc void @"?PrintList@?$GcList@VGcObjList@@@@QAEXXZ"(%class.GcList* @"?gc_obj_list@@3V?$GcList@VGcObjList@@@@A")
  ret void
}

; Function Attrs: noinline optnone
define linkonce_odr dso_local x86_thiscallcc void @"?PrintList@?$GcList@VGcObjList@@@@QAEXXZ"(%class.GcList*) #0 comdat align 2 {
  %2 = alloca %class.GcList*, align 4
  %3 = alloca %struct.GcObj*, align 4
  %4 = alloca %struct.GcObj*, align 4
  store %class.GcList* %0, %class.GcList** %2, align 4
  %5 = load %class.GcList*, %class.GcList** %2, align 4
  %6 = getelementptr inbounds %class.GcList, %class.GcList* %5, i32 0, i32 0
  %7 = load %struct.GcObj*, %struct.GcObj** %6, align 4
  store %struct.GcObj* %7, %struct.GcObj** %3, align 4
  store %struct.GcObj* null, %struct.GcObj** %4, align 4
  %8 = call i8* @"?ListName@GcObjList@@SAPBDXZ"()
  %9 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %10 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @"??_C@_09EBJFJNG@?$CFs?5start?6?$AA@", i32 0, i32 0), i8* %8)
  %11 = call i8* @"?ListName@GcObjList@@SAPBDXZ"()
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @"??_C@_09EBJFJNG@?$CFs?5start?6?$AA@", i32 0, i32 0), i8* %11)
  br label %13

; <label>:13:                                     ; preds = %51, %1
  %14 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %15 = icmp ne %struct.GcObj* %14, null
  br i1 %15, label %16, label %55

; <label>:16:                                     ; preds = %13
  %17 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %18 = ptrtoint %struct.GcObj* %17 to i32
  %19 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %20 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %19, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@", i32 0, i32 0), i32 %18)
  %21 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %22 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %21, i32 0, i32 4
  %23 = load i8, i8* %22, align 4
  %24 = trunc i8 %23 to i1
  %25 = zext i1 %24 to i32
  %26 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %27 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %26, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @"??_C@_0BD@HGGAKDEI@?5?5is_reachable?$DN?$CFd?6?$AA@", i32 0, i32 0), i32 %25)
  %28 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %29 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %28, i32 0, i32 5
  %30 = load i8*, i8** %29, align 4
  %31 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %32 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %31, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @"??_C@_0P@DPHCNOIJ@?5?5typename?$DN?$CFs?6?$AA@", i32 0, i32 0), i8* %30)
  %33 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @"??_C@_04MDPHBIK@obj?6?$AA@", i32 0, i32 0))
  %34 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %35 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %34, i32 0, i32 4
  %36 = load i8, i8* %35, align 4
  %37 = trunc i8 %36 to i1
  %38 = zext i1 %37 to i32
  %39 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @"??_C@_0BD@HGGAKDEI@?5?5is_reachable?$DN?$CFd?6?$AA@", i32 0, i32 0), i32 %38)
  %40 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %41 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %40, i32 0, i32 5
  %42 = load i8*, i8** %41, align 4
  %43 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @"??_C@_0P@DPHCNOIJ@?5?5typename?$DN?$CFs?6?$AA@", i32 0, i32 0), i8* %42)
  %44 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  %45 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %46 = call %struct.GcObj* @"?GetPrev@GcObjList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj* %45)
  %47 = icmp ne %struct.GcObj* %44, %46
  br i1 %47, label %48, label %51

; <label>:48:                                     ; preds = %16
  %49 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %50 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %49, i8* getelementptr inbounds ([46 x i8], [46 x i8]* @"??_C@_0CO@HGIHIH@?5?5BADBADBADBADBADBADBADBADBADBAD@", i32 0, i32 0))
  br label %51

; <label>:51:                                     ; preds = %48, %16
  %52 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  store %struct.GcObj* %52, %struct.GcObj** %4, align 4
  %53 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %54 = call %struct.GcObj* @"?GetNext@GcObjList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj* %53)
  store %struct.GcObj* %54, %struct.GcObj** %3, align 4
  br label %13

; <label>:55:                                     ; preds = %13
  %56 = call i8* @"?ListName@GcObjList@@SAPBDXZ"()
  %57 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %58 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %57, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @"??_C@_08BPFNJJP@?$CFs?5end?6?6?$AA@", i32 0, i32 0), i8* %56)
  %59 = call i8* @"?ListName@GcObjList@@SAPBDXZ"()
  %60 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @"??_C@_08BPFNJJP@?$CFs?5end?6?6?$AA@", i32 0, i32 0), i8* %59)
  ret void
}

; Function Attrs: noinline optnone
define dso_local i8* @gc_malloc(i32) #0 {
  %2 = alloca i32, align 4
  %3 = alloca %struct.GcObj*, align 4
  store i32 %0, i32* %2, align 4
  %4 = load i32, i32* %2, align 4
  %5 = call noalias i8* @malloc(i32 %4)
  %6 = bitcast i8* %5 to %struct.GcObj*
  store %struct.GcObj* %6, %struct.GcObj** %3, align 4
  %7 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %8 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %7, i32 0, i32 0
  store %struct.GcObj* null, %struct.GcObj** %8, align 4
  %9 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %10 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %9, i32 0, i32 1
  store %struct.GcObj* null, %struct.GcObj** %10, align 4
  %11 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %12 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %11, i32 0, i32 2
  store %struct.GcObj* null, %struct.GcObj** %12, align 4
  %13 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %14 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %13, i32 0, i32 3
  store %struct.GcObj* null, %struct.GcObj** %14, align 4
  %15 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %16 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %15, i32 0, i32 4
  store i8 0, i8* %16, align 4
  %17 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  call x86_thiscallcc void @"?PushFront@?$GcList@VGcObjList@@@@QAEXPAUGcObj@@@Z"(%class.GcList* @"?gc_obj_list@@3V?$GcList@VGcObjList@@@@A", %struct.GcObj* %17)
  %18 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %19 = bitcast %struct.GcObj* %18 to i8*
  ret i8* %19
}

declare dso_local noalias i8* @malloc(i32) #1

; Function Attrs: noinline optnone
define linkonce_odr dso_local x86_thiscallcc void @"?PushFront@?$GcList@VGcObjList@@@@QAEXPAUGcObj@@@Z"(%class.GcList*, %struct.GcObj*) #0 comdat align 2 {
  %3 = alloca %struct.GcObj*, align 4
  %4 = alloca %class.GcList*, align 4
  store %struct.GcObj* %1, %struct.GcObj** %3, align 4
  store %class.GcList* %0, %class.GcList** %4, align 4
  %5 = load %class.GcList*, %class.GcList** %4, align 4
  %6 = getelementptr inbounds %class.GcList, %class.GcList* %5, i32 0, i32 0
  %7 = load %struct.GcObj*, %struct.GcObj** %6, align 4
  %8 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  call void @"?SetNext@GcObjList@@SAXPAUGcObj@@0@Z"(%struct.GcObj* %8, %struct.GcObj* %7)
  %9 = getelementptr inbounds %class.GcList, %class.GcList* %5, i32 0, i32 0
  %10 = load %struct.GcObj*, %struct.GcObj** %9, align 4
  %11 = icmp ne %struct.GcObj* %10, null
  br i1 %11, label %12, label %16

; <label>:12:                                     ; preds = %2
  %13 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %14 = getelementptr inbounds %class.GcList, %class.GcList* %5, i32 0, i32 0
  %15 = load %struct.GcObj*, %struct.GcObj** %14, align 4
  call void @"?SetPrev@GcObjList@@SAXPAUGcObj@@0@Z"(%struct.GcObj* %15, %struct.GcObj* %13)
  br label %16

; <label>:16:                                     ; preds = %12, %2
  %17 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %18 = getelementptr inbounds %class.GcList, %class.GcList* %5, i32 0, i32 0
  store %struct.GcObj* %17, %struct.GcObj** %18, align 4
  ret void
}

; Function Attrs: noinline optnone
define dso_local void @gc_add_root(%struct.GcObj*) #0 {
  %2 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %0, %struct.GcObj** %2, align 4
  %3 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  call x86_thiscallcc void @"?PushFront@?$GcList@VGcRootList@@@@QAEXPAUGcObj@@@Z"(%class.GcList.0* @"?gc_root_list@@3V?$GcList@VGcRootList@@@@A", %struct.GcObj* %3)
  call x86_thiscallcc void @"?PrintList@?$GcList@VGcRootList@@@@QAEXXZ"(%class.GcList.0* @"?gc_root_list@@3V?$GcList@VGcRootList@@@@A")
  ret void
}

; Function Attrs: noinline optnone
define linkonce_odr dso_local x86_thiscallcc void @"?PushFront@?$GcList@VGcRootList@@@@QAEXPAUGcObj@@@Z"(%class.GcList.0*, %struct.GcObj*) #0 comdat align 2 {
  %3 = alloca %struct.GcObj*, align 4
  %4 = alloca %class.GcList.0*, align 4
  store %struct.GcObj* %1, %struct.GcObj** %3, align 4
  store %class.GcList.0* %0, %class.GcList.0** %4, align 4
  %5 = load %class.GcList.0*, %class.GcList.0** %4, align 4
  %6 = getelementptr inbounds %class.GcList.0, %class.GcList.0* %5, i32 0, i32 0
  %7 = load %struct.GcObj*, %struct.GcObj** %6, align 4
  %8 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  call void @"?SetNext@GcRootList@@SAXPAUGcObj@@0@Z"(%struct.GcObj* %8, %struct.GcObj* %7)
  %9 = getelementptr inbounds %class.GcList.0, %class.GcList.0* %5, i32 0, i32 0
  %10 = load %struct.GcObj*, %struct.GcObj** %9, align 4
  %11 = icmp ne %struct.GcObj* %10, null
  br i1 %11, label %12, label %16

; <label>:12:                                     ; preds = %2
  %13 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %14 = getelementptr inbounds %class.GcList.0, %class.GcList.0* %5, i32 0, i32 0
  %15 = load %struct.GcObj*, %struct.GcObj** %14, align 4
  call void @"?SetPrev@GcRootList@@SAXPAUGcObj@@0@Z"(%struct.GcObj* %15, %struct.GcObj* %13)
  br label %16

; <label>:16:                                     ; preds = %12, %2
  %17 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %18 = getelementptr inbounds %class.GcList.0, %class.GcList.0* %5, i32 0, i32 0
  store %struct.GcObj* %17, %struct.GcObj** %18, align 4
  ret void
}

; Function Attrs: noinline optnone
define linkonce_odr dso_local x86_thiscallcc void @"?PrintList@?$GcList@VGcRootList@@@@QAEXXZ"(%class.GcList.0*) #0 comdat align 2 {
  %2 = alloca %class.GcList.0*, align 4
  %3 = alloca %struct.GcObj*, align 4
  %4 = alloca %struct.GcObj*, align 4
  store %class.GcList.0* %0, %class.GcList.0** %2, align 4
  %5 = load %class.GcList.0*, %class.GcList.0** %2, align 4
  %6 = getelementptr inbounds %class.GcList.0, %class.GcList.0* %5, i32 0, i32 0
  %7 = load %struct.GcObj*, %struct.GcObj** %6, align 4
  store %struct.GcObj* %7, %struct.GcObj** %3, align 4
  store %struct.GcObj* null, %struct.GcObj** %4, align 4
  %8 = call i8* @"?ListName@GcRootList@@SAPBDXZ"()
  %9 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %10 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @"??_C@_09EBJFJNG@?$CFs?5start?6?$AA@", i32 0, i32 0), i8* %8)
  %11 = call i8* @"?ListName@GcRootList@@SAPBDXZ"()
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @"??_C@_09EBJFJNG@?$CFs?5start?6?$AA@", i32 0, i32 0), i8* %11)
  br label %13

; <label>:13:                                     ; preds = %51, %1
  %14 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %15 = icmp ne %struct.GcObj* %14, null
  br i1 %15, label %16, label %55

; <label>:16:                                     ; preds = %13
  %17 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %18 = ptrtoint %struct.GcObj* %17 to i32
  %19 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %20 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %19, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@", i32 0, i32 0), i32 %18)
  %21 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %22 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %21, i32 0, i32 4
  %23 = load i8, i8* %22, align 4
  %24 = trunc i8 %23 to i1
  %25 = zext i1 %24 to i32
  %26 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %27 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %26, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @"??_C@_0BD@HGGAKDEI@?5?5is_reachable?$DN?$CFd?6?$AA@", i32 0, i32 0), i32 %25)
  %28 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %29 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %28, i32 0, i32 5
  %30 = load i8*, i8** %29, align 4
  %31 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %32 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %31, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @"??_C@_0P@DPHCNOIJ@?5?5typename?$DN?$CFs?6?$AA@", i32 0, i32 0), i8* %30)
  %33 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @"??_C@_04MDPHBIK@obj?6?$AA@", i32 0, i32 0))
  %34 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %35 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %34, i32 0, i32 4
  %36 = load i8, i8* %35, align 4
  %37 = trunc i8 %36 to i1
  %38 = zext i1 %37 to i32
  %39 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @"??_C@_0BD@HGGAKDEI@?5?5is_reachable?$DN?$CFd?6?$AA@", i32 0, i32 0), i32 %38)
  %40 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %41 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %40, i32 0, i32 5
  %42 = load i8*, i8** %41, align 4
  %43 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @"??_C@_0P@DPHCNOIJ@?5?5typename?$DN?$CFs?6?$AA@", i32 0, i32 0), i8* %42)
  %44 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  %45 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %46 = call %struct.GcObj* @"?GetPrev@GcRootList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj* %45)
  %47 = icmp ne %struct.GcObj* %44, %46
  br i1 %47, label %48, label %51

; <label>:48:                                     ; preds = %16
  %49 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %50 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %49, i8* getelementptr inbounds ([46 x i8], [46 x i8]* @"??_C@_0CO@HGIHIH@?5?5BADBADBADBADBADBADBADBADBADBAD@", i32 0, i32 0))
  br label %51

; <label>:51:                                     ; preds = %48, %16
  %52 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  store %struct.GcObj* %52, %struct.GcObj** %4, align 4
  %53 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %54 = call %struct.GcObj* @"?GetNext@GcRootList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj* %53)
  store %struct.GcObj* %54, %struct.GcObj** %3, align 4
  br label %13

; <label>:55:                                     ; preds = %13
  %56 = call i8* @"?ListName@GcRootList@@SAPBDXZ"()
  %57 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %58 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %57, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @"??_C@_08BPFNJJP@?$CFs?5end?6?6?$AA@", i32 0, i32 0), i8* %56)
  %59 = call i8* @"?ListName@GcRootList@@SAPBDXZ"()
  %60 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @"??_C@_08BPFNJJP@?$CFs?5end?6?6?$AA@", i32 0, i32 0), i8* %59)
  ret void
}

; Function Attrs: noinline optnone
define dso_local void @gc_remove_root(%struct.GcObj*) #0 {
  %2 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %0, %struct.GcObj** %2, align 4
  %3 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  call x86_thiscallcc void @"?Remove@?$GcList@VGcRootList@@@@QAEXPAUGcObj@@@Z"(%class.GcList.0* @"?gc_root_list@@3V?$GcList@VGcRootList@@@@A", %struct.GcObj* %3)
  call x86_thiscallcc void @"?PrintList@?$GcList@VGcRootList@@@@QAEXXZ"(%class.GcList.0* @"?gc_root_list@@3V?$GcList@VGcRootList@@@@A")
  ret void
}

; Function Attrs: noinline optnone
define linkonce_odr dso_local x86_thiscallcc void @"?Remove@?$GcList@VGcRootList@@@@QAEXPAUGcObj@@@Z"(%class.GcList.0*, %struct.GcObj*) #0 comdat align 2 {
  %3 = alloca %struct.GcObj*, align 4
  %4 = alloca %class.GcList.0*, align 4
  %5 = alloca %struct.GcObj*, align 4
  %6 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %1, %struct.GcObj** %3, align 4
  store %class.GcList.0* %0, %class.GcList.0** %4, align 4
  %7 = load %class.GcList.0*, %class.GcList.0** %4, align 4
  %8 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %9 = call %struct.GcObj* @"?GetNext@GcRootList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj* %8)
  store %struct.GcObj* %9, %struct.GcObj** %5, align 4
  %10 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %11 = call %struct.GcObj* @"?GetPrev@GcRootList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj* %10)
  store %struct.GcObj* %11, %struct.GcObj** %6, align 4
  %12 = load %struct.GcObj*, %struct.GcObj** %5, align 4
  %13 = icmp eq %struct.GcObj* %12, null
  br i1 %13, label %14, label %23

; <label>:14:                                     ; preds = %2
  %15 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %16 = getelementptr inbounds %class.GcList.0, %class.GcList.0* %7, i32 0, i32 0
  %17 = load %struct.GcObj*, %struct.GcObj** %16, align 4
  %18 = icmp ne %struct.GcObj* %15, %17
  br i1 %18, label %19, label %23

; <label>:19:                                     ; preds = %14
  %20 = call i8* @"?ListName@GcRootList@@SAPBDXZ"()
  %21 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %22 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %21, i8* getelementptr inbounds ([47 x i8], [47 x i8]* @"??_C@_0CP@HIJCJBBI@Tried?5to?5remove?5an?5obj?5that?8s?5no@", i32 0, i32 0), i8* %20)
  br label %44

; <label>:23:                                     ; preds = %14, %2
  %24 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %25 = getelementptr inbounds %class.GcList.0, %class.GcList.0* %7, i32 0, i32 0
  %26 = load %struct.GcObj*, %struct.GcObj** %25, align 4
  %27 = icmp eq %struct.GcObj* %24, %26
  br i1 %27, label %28, label %32

; <label>:28:                                     ; preds = %23
  %29 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %30 = call %struct.GcObj* @"?GetNext@GcRootList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj* %29)
  %31 = getelementptr inbounds %class.GcList.0, %class.GcList.0* %7, i32 0, i32 0
  store %struct.GcObj* %30, %struct.GcObj** %31, align 4
  br label %32

; <label>:32:                                     ; preds = %28, %23
  %33 = load %struct.GcObj*, %struct.GcObj** %5, align 4
  %34 = icmp ne %struct.GcObj* %33, null
  br i1 %34, label %35, label %38

; <label>:35:                                     ; preds = %32
  %36 = load %struct.GcObj*, %struct.GcObj** %6, align 4
  %37 = load %struct.GcObj*, %struct.GcObj** %5, align 4
  call void @"?SetNext@GcRootList@@SAXPAUGcObj@@0@Z"(%struct.GcObj* %37, %struct.GcObj* %36)
  br label %38

; <label>:38:                                     ; preds = %35, %32
  %39 = load %struct.GcObj*, %struct.GcObj** %6, align 4
  %40 = icmp ne %struct.GcObj* %39, null
  br i1 %40, label %41, label %44

; <label>:41:                                     ; preds = %38
  %42 = load %struct.GcObj*, %struct.GcObj** %5, align 4
  %43 = load %struct.GcObj*, %struct.GcObj** %6, align 4
  call void @"?SetPrev@GcRootList@@SAXPAUGcObj@@0@Z"(%struct.GcObj* %43, %struct.GcObj* %42)
  br label %44

; <label>:44:                                     ; preds = %19, %41, %38
  ret void
}

; Function Attrs: noinline optnone
define linkonce_odr dso_local i32 @fprintf(%struct._iobuf*, i8*, ...) #0 comdat {
  %3 = alloca i8*, align 4
  %4 = alloca %struct._iobuf*, align 4
  %5 = alloca i32, align 4
  %6 = alloca i8*, align 4
  store i8* %1, i8** %3, align 4
  store %struct._iobuf* %0, %struct._iobuf** %4, align 4
  %7 = bitcast i8** %6 to i8*
  call void @llvm.va_start(i8* %7)
  %8 = load i8*, i8** %6, align 4
  %9 = load i8*, i8** %3, align 4
  %10 = load %struct._iobuf*, %struct._iobuf** %4, align 4
  %11 = call i32 @_vfprintf_l(%struct._iobuf* %10, i8* %9, %struct.__crt_locale_pointers* null, i8* %8)
  store i32 %11, i32* %5, align 4
  %12 = bitcast i8** %6 to i8*
  call void @llvm.va_end(i8* %12)
  %13 = load i32, i32* %5, align 4
  ret i32 %13
}

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local i8* @"?ListName@GcObjList@@SAPBDXZ"() #2 comdat align 2 {
  ret i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"??_C@_07DGBOGBKN@gc?5objs?$AA@", i32 0, i32 0)
}

declare dso_local %struct._iobuf* @__acrt_iob_func(i32) #1

; Function Attrs: noinline optnone
define linkonce_odr dso_local i32 @printf(i8*, ...) #0 comdat {
  %2 = alloca i8*, align 4
  %3 = alloca i32, align 4
  %4 = alloca i8*, align 4
  store i8* %0, i8** %2, align 4
  %5 = bitcast i8** %4 to i8*
  call void @llvm.va_start(i8* %5)
  %6 = load i8*, i8** %4, align 4
  %7 = load i8*, i8** %2, align 4
  %8 = call %struct._iobuf* @__acrt_iob_func(i32 1)
  %9 = call i32 @_vfprintf_l(%struct._iobuf* %8, i8* %7, %struct.__crt_locale_pointers* null, i8* %6)
  store i32 %9, i32* %3, align 4
  %10 = bitcast i8** %4 to i8*
  call void @llvm.va_end(i8* %10)
  %11 = load i32, i32* %3, align 4
  ret i32 %11
}

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local %struct.GcObj* @"?GetPrev@GcObjList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj*) #2 comdat align 2 {
  %2 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %0, %struct.GcObj** %2, align 4
  %3 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %4 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %3, i32 0, i32 1
  %5 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  ret %struct.GcObj* %5
}

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local %struct.GcObj* @"?GetNext@GcObjList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj*) #2 comdat align 2 {
  %2 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %0, %struct.GcObj** %2, align 4
  %3 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %4 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %3, i32 0, i32 0
  %5 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  ret %struct.GcObj* %5
}

; Function Attrs: nounwind
declare void @llvm.va_start(i8*) #3

; Function Attrs: noinline optnone
define linkonce_odr dso_local i32 @_vfprintf_l(%struct._iobuf*, i8*, %struct.__crt_locale_pointers*, i8*) #0 comdat {
  %5 = alloca i8*, align 4
  %6 = alloca %struct.__crt_locale_pointers*, align 4
  %7 = alloca i8*, align 4
  %8 = alloca %struct._iobuf*, align 4
  store i8* %3, i8** %5, align 4
  store %struct.__crt_locale_pointers* %2, %struct.__crt_locale_pointers** %6, align 4
  store i8* %1, i8** %7, align 4
  store %struct._iobuf* %0, %struct._iobuf** %8, align 4
  %9 = load i8*, i8** %5, align 4
  %10 = load %struct.__crt_locale_pointers*, %struct.__crt_locale_pointers** %6, align 4
  %11 = load i8*, i8** %7, align 4
  %12 = load %struct._iobuf*, %struct._iobuf** %8, align 4
  %13 = call i64* @__local_stdio_printf_options()
  %14 = load i64, i64* %13, align 8
  %15 = call i32 @__stdio_common_vfprintf(i64 %14, %struct._iobuf* %12, i8* %11, %struct.__crt_locale_pointers* %10, i8* %9)
  ret i32 %15
}

; Function Attrs: nounwind
declare void @llvm.va_end(i8*) #3

declare dso_local i32 @__stdio_common_vfprintf(i64, %struct._iobuf*, i8*, %struct.__crt_locale_pointers*, i8*) #1

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local i64* @__local_stdio_printf_options() #2 comdat {
  ret i64* @"?_OptionsStorage@?1??__local_stdio_printf_options@@9@4_KA"
}

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local void @"?SetNext@GcObjList@@SAXPAUGcObj@@0@Z"(%struct.GcObj*, %struct.GcObj*) #2 comdat align 2 {
  %3 = alloca %struct.GcObj*, align 4
  %4 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %1, %struct.GcObj** %3, align 4
  store %struct.GcObj* %0, %struct.GcObj** %4, align 4
  %5 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %6 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  %7 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %6, i32 0, i32 0
  store %struct.GcObj* %5, %struct.GcObj** %7, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local void @"?SetPrev@GcObjList@@SAXPAUGcObj@@0@Z"(%struct.GcObj*, %struct.GcObj*) #2 comdat align 2 {
  %3 = alloca %struct.GcObj*, align 4
  %4 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %1, %struct.GcObj** %3, align 4
  store %struct.GcObj* %0, %struct.GcObj** %4, align 4
  %5 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %6 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  %7 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %6, i32 0, i32 1
  store %struct.GcObj* %5, %struct.GcObj** %7, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local void @"?SetNext@GcRootList@@SAXPAUGcObj@@0@Z"(%struct.GcObj*, %struct.GcObj*) #2 comdat align 2 {
  %3 = alloca %struct.GcObj*, align 4
  %4 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %1, %struct.GcObj** %3, align 4
  store %struct.GcObj* %0, %struct.GcObj** %4, align 4
  %5 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %6 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  %7 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %6, i32 0, i32 2
  store %struct.GcObj* %5, %struct.GcObj** %7, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local void @"?SetPrev@GcRootList@@SAXPAUGcObj@@0@Z"(%struct.GcObj*, %struct.GcObj*) #2 comdat align 2 {
  %3 = alloca %struct.GcObj*, align 4
  %4 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %1, %struct.GcObj** %3, align 4
  store %struct.GcObj* %0, %struct.GcObj** %4, align 4
  %5 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %6 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  %7 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %6, i32 0, i32 3
  store %struct.GcObj* %5, %struct.GcObj** %7, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local i8* @"?ListName@GcRootList@@SAPBDXZ"() #2 comdat align 2 {
  ret i8* getelementptr inbounds ([9 x i8], [9 x i8]* @"??_C@_08NJAKNIEC@gc?5roots?$AA@", i32 0, i32 0)
}

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local %struct.GcObj* @"?GetPrev@GcRootList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj*) #2 comdat align 2 {
  %2 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %0, %struct.GcObj** %2, align 4
  %3 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %4 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %3, i32 0, i32 3
  %5 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  ret %struct.GcObj* %5
}

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local %struct.GcObj* @"?GetNext@GcRootList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj*) #2 comdat align 2 {
  %2 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %0, %struct.GcObj** %2, align 4
  %3 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %4 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %3, i32 0, i32 2
  %5 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  ret %struct.GcObj* %5
}

attributes #0 = { noinline optnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { noinline nounwind optnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.linker.options = !{!0, !1, !2, !3, !4}
!llvm.module.flags = !{!5, !6}
!llvm.ident = !{!7}

!0 = !{!"/FAILIFMISMATCH:\22_MSC_VER=1900\22"}
!1 = !{!"/FAILIFMISMATCH:\22_ITERATOR_DEBUG_LEVEL=0\22"}
!2 = !{!"/FAILIFMISMATCH:\22RuntimeLibrary=MT_StaticRelease\22"}
!3 = !{!"/DEFAULTLIB:libcpmt.lib"}
!4 = !{!"/FAILIFMISMATCH:\22_CRT_STDIO_ISO_WIDE_SPECIFIERS=0\22"}
!5 = !{i32 1, !"NumRegisterParameters", i32 0}
!6 = !{i32 1, !"wchar_size", i32 2}
!7 = !{!"clang version 7.0.0 (tags/RELEASE_700/final)"}
