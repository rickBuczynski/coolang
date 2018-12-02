; ModuleID = '.\gc.cpp'
source_filename = ".\5Cgc.cpp"
target datalayout = "e-m:x-p:32:32-i64:64-f80:32-n8:16:32-a:0:32-S32"
target triple = "i386-pc-windows-msvc19.16.27024"

%class.GcList = type { %struct.GcObj* }
%struct.GcObj = type { %struct.GcObj*, %struct.GcObj*, %struct.GcObj*, %struct.GcObj*, i8, i8* }
%struct.GcRootStack = type { %struct.GcObj***, i32, i32 }
%struct._iobuf = type { i8* }
%struct.__crt_locale_pointers = type { %struct.__crt_locale_data*, %struct.__crt_multibyte_data* }
%struct.__crt_locale_data = type opaque
%struct.__crt_multibyte_data = type opaque

$fprintf = comdat any

$printf = comdat any

$"??0GcRootStack@@QAE@XZ" = comdat any

$"?PrintList@?$GcList@VGcObjList@@@@QAEXXZ" = comdat any

$"?PushFront@?$GcList@VGcObjList@@@@QAEXPAUGcObj@@@Z" = comdat any

$"?PushRoot@GcRootStack@@QAEXPAPAUGcObj@@@Z" = comdat any

$"?PrintRoots@GcRootStack@@QBEXXZ" = comdat any

$"?PopRoot@GcRootStack@@QAEXPAPAUGcObj@@@Z" = comdat any

$_vfprintf_l = comdat any

$__local_stdio_printf_options = comdat any

$"?ListName@GcObjList@@SAPBDXZ" = comdat any

$"?GetPrev@GcObjList@@SAPAUGcObj@@PAU2@@Z" = comdat any

$"?GetNext@GcObjList@@SAPAUGcObj@@PAU2@@Z" = comdat any

$"?SetNext@GcObjList@@SAXPAUGcObj@@0@Z" = comdat any

$"?SetPrev@GcObjList@@SAXPAUGcObj@@0@Z" = comdat any

$"??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@" = comdat any

$"??_C@_0BD@HGGAKDEI@?5?5is_reachable?$DN?$CFd?6?$AA@" = comdat any

$"??_C@_0P@DPHCNOIJ@?5?5typename?$DN?$CFs?6?$AA@" = comdat any

$"??_C@_04MDPHBIK@obj?6?$AA@" = comdat any

$"??_C@_0CC@HKCJKJMI@Inserting?5a?5root?5that?5points?5to?3@" = comdat any

$"??_C@_01EEMJAFIK@?6?$AA@" = comdat any

$"??_C@_0P@INCDPCBK@before?5insert?6?$AA@" = comdat any

$"??_C@_0O@EHGKPBHI@after?5insert?6?$AA@" = comdat any

$"??_C@_0CB@GKEBEHJL@Removing?5a?5root?5that?5points?5to?3?6@" = comdat any

$"?_OptionsStorage@?1??__local_stdio_printf_options@@9@4_KA" = comdat any

$"??_C@_0BE@BMNDJLDE@?6Current?5GC?5roots?3?6?$AA@" = comdat any

$"??_C@_0BG@FMOJGAOI@Root?5that?5points?5to?3?6?$AA@" = comdat any

$"??_C@_0BK@ONKPCMH@End?5of?5current?5GC?5roots?6?6?$AA@" = comdat any

$"??_C@_0BN@DLIALJMC@BADBADBADBADBADBADBADBADBAD?6?$AA@" = comdat any

$"??_C@_0CB@NGCPCCJL@Root?5at?5top?5of?5stack?5points?5to?3?6@" = comdat any

$"??_C@_0BL@PDCIMNAE@Root?5to?5remove?5points?5to?3?6?$AA@" = comdat any

$"??_C@_09EBJFJNG@?$CFs?5start?6?$AA@" = comdat any

$"??_C@_0CO@HGIHIH@?5?5BADBADBADBADBADBADBADBADBADBAD@" = comdat any

$"??_C@_08BPFNJJP@?$CFs?5end?6?6?$AA@" = comdat any

$"??_C@_07DGBOGBKN@gc?5objs?$AA@" = comdat any

@"??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [8 x i8] c"obj=%d\0A\00", comdat, align 1
@"??_C@_0BD@HGGAKDEI@?5?5is_reachable?$DN?$CFd?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [19 x i8] c"  is_reachable=%d\0A\00", comdat, align 1
@"??_C@_0P@DPHCNOIJ@?5?5typename?$DN?$CFs?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [15 x i8] c"  typename=%s\0A\00", comdat, align 1
@"??_C@_04MDPHBIK@obj?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [5 x i8] c"obj\0A\00", comdat, align 1
@"?gc_obj_list@@3V?$GcList@VGcObjList@@@@A" = dso_local global %class.GcList zeroinitializer, align 4
@"?gc_roots@@3UGcRootStack@@A" = dso_local global %struct.GcRootStack zeroinitializer, align 4
@"??_C@_0CC@HKCJKJMI@Inserting?5a?5root?5that?5points?5to?3@" = linkonce_odr dso_local unnamed_addr constant [34 x i8] c"Inserting a root that points to:\0A\00", comdat, align 1
@"??_C@_01EEMJAFIK@?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [2 x i8] c"\0A\00", comdat, align 1
@"??_C@_0P@INCDPCBK@before?5insert?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [15 x i8] c"before insert\0A\00", comdat, align 1
@"??_C@_0O@EHGKPBHI@after?5insert?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [14 x i8] c"after insert\0A\00", comdat, align 1
@"??_C@_0CB@GKEBEHJL@Removing?5a?5root?5that?5points?5to?3?6@" = linkonce_odr dso_local unnamed_addr constant [33 x i8] c"Removing a root that points to:\0A\00", comdat, align 1
@"?_OptionsStorage@?1??__local_stdio_printf_options@@9@4_KA" = linkonce_odr dso_local global i64 0, comdat, align 8
@"??_C@_0BE@BMNDJLDE@?6Current?5GC?5roots?3?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [20 x i8] c"\0ACurrent GC roots:\0A\00", comdat, align 1
@"??_C@_0BG@FMOJGAOI@Root?5that?5points?5to?3?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [22 x i8] c"Root that points to:\0A\00", comdat, align 1
@"??_C@_0BK@ONKPCMH@End?5of?5current?5GC?5roots?6?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [26 x i8] c"End of current GC roots\0A\0A\00", comdat, align 1
@"??_C@_0BN@DLIALJMC@BADBADBADBADBADBADBADBADBAD?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [29 x i8] c"BADBADBADBADBADBADBADBADBAD\0A\00", comdat, align 1
@"??_C@_0CB@NGCPCCJL@Root?5at?5top?5of?5stack?5points?5to?3?6@" = linkonce_odr dso_local unnamed_addr constant [33 x i8] c"Root at top of stack points to:\0A\00", comdat, align 1
@"??_C@_0BL@PDCIMNAE@Root?5to?5remove?5points?5to?3?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [27 x i8] c"Root to remove points to:\0A\00", comdat, align 1
@"??_C@_09EBJFJNG@?$CFs?5start?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [10 x i8] c"%s start\0A\00", comdat, align 1
@"??_C@_0CO@HGIHIH@?5?5BADBADBADBADBADBADBADBADBADBAD@" = linkonce_odr dso_local unnamed_addr constant [46 x i8] c"  BADBADBADBADBADBADBADBADBADBADBADBADBADBAD\0A\00", comdat, align 1
@"??_C@_08BPFNJJP@?$CFs?5end?6?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [9 x i8] c"%s end\0A\0A\00", comdat, align 1
@"??_C@_07DGBOGBKN@gc?5objs?$AA@" = linkonce_odr dso_local unnamed_addr constant [8 x i8] c"gc objs\00", comdat, align 1
@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 65535, void ()* @_GLOBAL__sub_I_gc.cpp, i8* null }]

; Function Attrs: noinline optnone
define dso_local void @"?PrintObj@@YAXPAUGcObj@@@Z"(%struct.GcObj*) #0 {
  %2 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %0, %struct.GcObj** %2, align 4
  %3 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %4 = ptrtoint %struct.GcObj* %3 to i32
  %5 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %6 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %5, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@", i32 0, i32 0), i32 %4)
  %7 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %8 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %7, i32 0, i32 4
  %9 = load i8, i8* %8, align 4
  %10 = trunc i8 %9 to i1
  %11 = zext i1 %10 to i32
  %12 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %13 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %12, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @"??_C@_0BD@HGGAKDEI@?5?5is_reachable?$DN?$CFd?6?$AA@", i32 0, i32 0), i32 %11)
  %14 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %15 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %14, i32 0, i32 5
  %16 = load i8*, i8** %15, align 4
  %17 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %18 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %17, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @"??_C@_0P@DPHCNOIJ@?5?5typename?$DN?$CFs?6?$AA@", i32 0, i32 0), i8* %16)
  %19 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @"??_C@_04MDPHBIK@obj?6?$AA@", i32 0, i32 0))
  %20 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %21 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %20, i32 0, i32 4
  %22 = load i8, i8* %21, align 4
  %23 = trunc i8 %22 to i1
  %24 = zext i1 %23 to i32
  %25 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @"??_C@_0BD@HGGAKDEI@?5?5is_reachable?$DN?$CFd?6?$AA@", i32 0, i32 0), i32 %24)
  %26 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %27 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %26, i32 0, i32 5
  %28 = load i8*, i8** %27, align 4
  %29 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @"??_C@_0P@DPHCNOIJ@?5?5typename?$DN?$CFs?6?$AA@", i32 0, i32 0), i8* %28)
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

; Function Attrs: noinline
define internal void @"??__Egc_roots@@YAXXZ"() #2 {
  %1 = call x86_thiscallcc %struct.GcRootStack* @"??0GcRootStack@@QAE@XZ"(%struct.GcRootStack* @"?gc_roots@@3UGcRootStack@@A")
  ret void
}

; Function Attrs: noinline optnone
define linkonce_odr dso_local x86_thiscallcc %struct.GcRootStack* @"??0GcRootStack@@QAE@XZ"(%struct.GcRootStack* returned) unnamed_addr #0 comdat align 2 {
  %2 = alloca %struct.GcRootStack*, align 4
  store %struct.GcRootStack* %0, %struct.GcRootStack** %2, align 4
  %3 = load %struct.GcRootStack*, %struct.GcRootStack** %2, align 4
  %4 = call noalias i8* @malloc(i32 8)
  %5 = bitcast i8* %4 to %struct.GcObj***
  %6 = getelementptr inbounds %struct.GcRootStack, %struct.GcRootStack* %3, i32 0, i32 0
  store %struct.GcObj*** %5, %struct.GcObj**** %6, align 4
  %7 = getelementptr inbounds %struct.GcRootStack, %struct.GcRootStack* %3, i32 0, i32 1
  store i32 2, i32* %7, align 4
  %8 = getelementptr inbounds %struct.GcRootStack, %struct.GcRootStack* %3, i32 0, i32 2
  store i32 0, i32* %8, align 4
  ret %struct.GcRootStack* %3
}

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

; <label>:13:                                     ; preds = %25, %1
  %14 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %15 = icmp ne %struct.GcObj* %14, null
  br i1 %15, label %16, label %29

; <label>:16:                                     ; preds = %13
  %17 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  call void @"?PrintObj@@YAXPAUGcObj@@@Z"(%struct.GcObj* %17)
  %18 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  %19 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %20 = call %struct.GcObj* @"?GetPrev@GcObjList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj* %19)
  %21 = icmp ne %struct.GcObj* %18, %20
  br i1 %21, label %22, label %25

; <label>:22:                                     ; preds = %16
  %23 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %24 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %23, i8* getelementptr inbounds ([46 x i8], [46 x i8]* @"??_C@_0CO@HGIHIH@?5?5BADBADBADBADBADBADBADBADBADBAD@", i32 0, i32 0))
  br label %25

; <label>:25:                                     ; preds = %22, %16
  %26 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  store %struct.GcObj* %26, %struct.GcObj** %4, align 4
  %27 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %28 = call %struct.GcObj* @"?GetNext@GcObjList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj* %27)
  store %struct.GcObj* %28, %struct.GcObj** %3, align 4
  br label %13

; <label>:29:                                     ; preds = %13
  %30 = call i8* @"?ListName@GcObjList@@SAPBDXZ"()
  %31 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %32 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %31, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @"??_C@_08BPFNJJP@?$CFs?5end?6?6?$AA@", i32 0, i32 0), i8* %30)
  %33 = call i8* @"?ListName@GcObjList@@SAPBDXZ"()
  %34 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @"??_C@_08BPFNJJP@?$CFs?5end?6?6?$AA@", i32 0, i32 0), i8* %33)
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
define dso_local void @gc_add_root(%struct.GcObj**) #0 {
  %2 = alloca %struct.GcObj**, align 4
  store %struct.GcObj** %0, %struct.GcObj*** %2, align 4
  %3 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %4 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %3, i8* getelementptr inbounds ([34 x i8], [34 x i8]* @"??_C@_0CC@HKCJKJMI@Inserting?5a?5root?5that?5points?5to?3@", i32 0, i32 0))
  %5 = load %struct.GcObj**, %struct.GcObj*** %2, align 4
  %6 = load %struct.GcObj*, %struct.GcObj** %5, align 4
  call void @"?PrintObj@@YAXPAUGcObj@@@Z"(%struct.GcObj* %6)
  %7 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %8 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %7, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @"??_C@_01EEMJAFIK@?6?$AA@", i32 0, i32 0))
  %9 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %10 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %9, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @"??_C@_0P@INCDPCBK@before?5insert?6?$AA@", i32 0, i32 0))
  %11 = load %struct.GcObj**, %struct.GcObj*** %2, align 4
  call x86_thiscallcc void @"?PushRoot@GcRootStack@@QAEXPAPAUGcObj@@@Z"(%struct.GcRootStack* @"?gc_roots@@3UGcRootStack@@A", %struct.GcObj** %11)
  %12 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %13 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %12, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @"??_C@_0O@EHGKPBHI@after?5insert?6?$AA@", i32 0, i32 0))
  call x86_thiscallcc void @"?PrintRoots@GcRootStack@@QBEXXZ"(%struct.GcRootStack* @"?gc_roots@@3UGcRootStack@@A")
  ret void
}

; Function Attrs: noinline optnone
define linkonce_odr dso_local x86_thiscallcc void @"?PushRoot@GcRootStack@@QAEXPAPAUGcObj@@@Z"(%struct.GcRootStack*, %struct.GcObj**) #0 comdat align 2 {
  %3 = alloca %struct.GcObj**, align 4
  %4 = alloca %struct.GcRootStack*, align 4
  store %struct.GcObj** %1, %struct.GcObj*** %3, align 4
  store %struct.GcRootStack* %0, %struct.GcRootStack** %4, align 4
  %5 = load %struct.GcRootStack*, %struct.GcRootStack** %4, align 4
  %6 = getelementptr inbounds %struct.GcRootStack, %struct.GcRootStack* %5, i32 0, i32 2
  %7 = load i32, i32* %6, align 4
  %8 = getelementptr inbounds %struct.GcRootStack, %struct.GcRootStack* %5, i32 0, i32 1
  %9 = load i32, i32* %8, align 4
  %10 = icmp eq i32 %7, %9
  br i1 %10, label %11, label %23

; <label>:11:                                     ; preds = %2
  %12 = getelementptr inbounds %struct.GcRootStack, %struct.GcRootStack* %5, i32 0, i32 1
  %13 = load i32, i32* %12, align 4
  %14 = mul nsw i32 %13, 2
  store i32 %14, i32* %12, align 4
  %15 = getelementptr inbounds %struct.GcRootStack, %struct.GcRootStack* %5, i32 0, i32 1
  %16 = load i32, i32* %15, align 4
  %17 = getelementptr inbounds %struct.GcRootStack, %struct.GcRootStack* %5, i32 0, i32 0
  %18 = load %struct.GcObj***, %struct.GcObj**** %17, align 4
  %19 = bitcast %struct.GcObj*** %18 to i8*
  %20 = call noalias i8* @realloc(i8* %19, i32 %16)
  %21 = bitcast i8* %20 to %struct.GcObj***
  %22 = getelementptr inbounds %struct.GcRootStack, %struct.GcRootStack* %5, i32 0, i32 0
  store %struct.GcObj*** %21, %struct.GcObj**** %22, align 4
  br label %23

; <label>:23:                                     ; preds = %11, %2
  %24 = load %struct.GcObj**, %struct.GcObj*** %3, align 4
  %25 = getelementptr inbounds %struct.GcRootStack, %struct.GcRootStack* %5, i32 0, i32 0
  %26 = load %struct.GcObj***, %struct.GcObj**** %25, align 4
  %27 = getelementptr inbounds %struct.GcRootStack, %struct.GcRootStack* %5, i32 0, i32 2
  %28 = load i32, i32* %27, align 4
  %29 = getelementptr inbounds %struct.GcObj**, %struct.GcObj*** %26, i32 %28
  store %struct.GcObj** %24, %struct.GcObj*** %29, align 4
  %30 = getelementptr inbounds %struct.GcRootStack, %struct.GcRootStack* %5, i32 0, i32 2
  %31 = load i32, i32* %30, align 4
  %32 = add nsw i32 %31, 1
  store i32 %32, i32* %30, align 4
  ret void
}

; Function Attrs: noinline optnone
define linkonce_odr dso_local x86_thiscallcc void @"?PrintRoots@GcRootStack@@QBEXXZ"(%struct.GcRootStack*) #0 comdat align 2 {
  %2 = alloca %struct.GcRootStack*, align 4
  %3 = alloca i32, align 4
  store %struct.GcRootStack* %0, %struct.GcRootStack** %2, align 4
  %4 = load %struct.GcRootStack*, %struct.GcRootStack** %2, align 4
  %5 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %6 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %5, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @"??_C@_0BE@BMNDJLDE@?6Current?5GC?5roots?3?6?$AA@", i32 0, i32 0))
  store i32 0, i32* %3, align 4
  br label %7

; <label>:7:                                      ; preds = %21, %1
  %8 = load i32, i32* %3, align 4
  %9 = getelementptr inbounds %struct.GcRootStack, %struct.GcRootStack* %4, i32 0, i32 2
  %10 = load i32, i32* %9, align 4
  %11 = icmp slt i32 %8, %10
  br i1 %11, label %12, label %24

; <label>:12:                                     ; preds = %7
  %13 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %14 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %13, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @"??_C@_0BG@FMOJGAOI@Root?5that?5points?5to?3?6?$AA@", i32 0, i32 0))
  %15 = getelementptr inbounds %struct.GcRootStack, %struct.GcRootStack* %4, i32 0, i32 0
  %16 = load %struct.GcObj***, %struct.GcObj**** %15, align 4
  %17 = load i32, i32* %3, align 4
  %18 = getelementptr inbounds %struct.GcObj**, %struct.GcObj*** %16, i32 %17
  %19 = load %struct.GcObj**, %struct.GcObj*** %18, align 4
  %20 = load %struct.GcObj*, %struct.GcObj** %19, align 4
  call void @"?PrintObj@@YAXPAUGcObj@@@Z"(%struct.GcObj* %20)
  br label %21

; <label>:21:                                     ; preds = %12
  %22 = load i32, i32* %3, align 4
  %23 = add nsw i32 %22, 1
  store i32 %23, i32* %3, align 4
  br label %7

; <label>:24:                                     ; preds = %7
  %25 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %26 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %25, i8* getelementptr inbounds ([26 x i8], [26 x i8]* @"??_C@_0BK@ONKPCMH@End?5of?5current?5GC?5roots?6?6?$AA@", i32 0, i32 0))
  ret void
}

; Function Attrs: noinline optnone
define dso_local void @gc_remove_root(%struct.GcObj**) #0 {
  %2 = alloca %struct.GcObj**, align 4
  store %struct.GcObj** %0, %struct.GcObj*** %2, align 4
  %3 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %4 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %3, i8* getelementptr inbounds ([33 x i8], [33 x i8]* @"??_C@_0CB@GKEBEHJL@Removing?5a?5root?5that?5points?5to?3?6@", i32 0, i32 0))
  %5 = load %struct.GcObj**, %struct.GcObj*** %2, align 4
  %6 = load %struct.GcObj*, %struct.GcObj** %5, align 4
  call void @"?PrintObj@@YAXPAUGcObj@@@Z"(%struct.GcObj* %6)
  %7 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %8 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %7, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @"??_C@_01EEMJAFIK@?6?$AA@", i32 0, i32 0))
  %9 = load %struct.GcObj**, %struct.GcObj*** %2, align 4
  call x86_thiscallcc void @"?PopRoot@GcRootStack@@QAEXPAPAUGcObj@@@Z"(%struct.GcRootStack* @"?gc_roots@@3UGcRootStack@@A", %struct.GcObj** %9)
  call x86_thiscallcc void @"?PrintRoots@GcRootStack@@QBEXXZ"(%struct.GcRootStack* @"?gc_roots@@3UGcRootStack@@A")
  ret void
}

; Function Attrs: noinline optnone
define linkonce_odr dso_local x86_thiscallcc void @"?PopRoot@GcRootStack@@QAEXPAPAUGcObj@@@Z"(%struct.GcRootStack*, %struct.GcObj**) #0 comdat align 2 {
  %3 = alloca %struct.GcObj**, align 4
  %4 = alloca %struct.GcRootStack*, align 4
  store %struct.GcObj** %1, %struct.GcObj*** %3, align 4
  store %struct.GcRootStack* %0, %struct.GcRootStack** %4, align 4
  %5 = load %struct.GcRootStack*, %struct.GcRootStack** %4, align 4
  %6 = getelementptr inbounds %struct.GcRootStack, %struct.GcRootStack* %5, i32 0, i32 0
  %7 = load %struct.GcObj***, %struct.GcObj**** %6, align 4
  %8 = getelementptr inbounds %struct.GcRootStack, %struct.GcRootStack* %5, i32 0, i32 2
  %9 = load i32, i32* %8, align 4
  %10 = sub nsw i32 %9, 1
  %11 = getelementptr inbounds %struct.GcObj**, %struct.GcObj*** %7, i32 %10
  %12 = load %struct.GcObj**, %struct.GcObj*** %11, align 4
  %13 = load %struct.GcObj**, %struct.GcObj*** %3, align 4
  %14 = icmp ne %struct.GcObj** %12, %13
  br i1 %14, label %15, label %31

; <label>:15:                                     ; preds = %2
  %16 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %17 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %16, i8* getelementptr inbounds ([29 x i8], [29 x i8]* @"??_C@_0BN@DLIALJMC@BADBADBADBADBADBADBADBADBAD?6?$AA@", i32 0, i32 0))
  %18 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %19 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %18, i8* getelementptr inbounds ([33 x i8], [33 x i8]* @"??_C@_0CB@NGCPCCJL@Root?5at?5top?5of?5stack?5points?5to?3?6@", i32 0, i32 0))
  %20 = getelementptr inbounds %struct.GcRootStack, %struct.GcRootStack* %5, i32 0, i32 0
  %21 = load %struct.GcObj***, %struct.GcObj**** %20, align 4
  %22 = getelementptr inbounds %struct.GcRootStack, %struct.GcRootStack* %5, i32 0, i32 2
  %23 = load i32, i32* %22, align 4
  %24 = getelementptr inbounds %struct.GcObj**, %struct.GcObj*** %21, i32 %23
  %25 = load %struct.GcObj**, %struct.GcObj*** %24, align 4
  %26 = load %struct.GcObj*, %struct.GcObj** %25, align 4
  call void @"?PrintObj@@YAXPAUGcObj@@@Z"(%struct.GcObj* %26)
  %27 = call %struct._iobuf* @__acrt_iob_func(i32 2)
  %28 = call i32 (%struct._iobuf*, i8*, ...) @fprintf(%struct._iobuf* %27, i8* getelementptr inbounds ([27 x i8], [27 x i8]* @"??_C@_0BL@PDCIMNAE@Root?5to?5remove?5points?5to?3?6?$AA@", i32 0, i32 0))
  %29 = load %struct.GcObj**, %struct.GcObj*** %3, align 4
  %30 = load %struct.GcObj*, %struct.GcObj** %29, align 4
  call void @"?PrintObj@@YAXPAUGcObj@@@Z"(%struct.GcObj* %30)
  br label %31

; <label>:31:                                     ; preds = %15, %2
  %32 = getelementptr inbounds %struct.GcRootStack, %struct.GcRootStack* %5, i32 0, i32 2
  %33 = load i32, i32* %32, align 4
  %34 = add nsw i32 %33, -1
  store i32 %34, i32* %32, align 4
  ret void
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
define linkonce_odr dso_local i64* @__local_stdio_printf_options() #4 comdat {
  ret i64* @"?_OptionsStorage@?1??__local_stdio_printf_options@@9@4_KA"
}

declare dso_local noalias i8* @realloc(i8*, i32) #1

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local i8* @"?ListName@GcObjList@@SAPBDXZ"() #4 comdat align 2 {
  ret i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"??_C@_07DGBOGBKN@gc?5objs?$AA@", i32 0, i32 0)
}

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local %struct.GcObj* @"?GetPrev@GcObjList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj*) #4 comdat align 2 {
  %2 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %0, %struct.GcObj** %2, align 4
  %3 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %4 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %3, i32 0, i32 1
  %5 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  ret %struct.GcObj* %5
}

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local %struct.GcObj* @"?GetNext@GcObjList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj*) #4 comdat align 2 {
  %2 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %0, %struct.GcObj** %2, align 4
  %3 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %4 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %3, i32 0, i32 0
  %5 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  ret %struct.GcObj* %5
}

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local void @"?SetNext@GcObjList@@SAXPAUGcObj@@0@Z"(%struct.GcObj*, %struct.GcObj*) #4 comdat align 2 {
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
define linkonce_odr dso_local void @"?SetPrev@GcObjList@@SAXPAUGcObj@@0@Z"(%struct.GcObj*, %struct.GcObj*) #4 comdat align 2 {
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

; Function Attrs: noinline
define internal void @_GLOBAL__sub_I_gc.cpp() #2 {
  call void @"??__Egc_roots@@YAXXZ"()
  ret void
}

attributes #0 = { noinline optnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { noinline "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }
attributes #4 = { noinline nounwind optnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

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
