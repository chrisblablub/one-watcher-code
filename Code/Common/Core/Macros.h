/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef MACROS_H
#define MACROS_H

#ifdef ENV_PLATFORM_WINDOWS
#define ENV_PLATFORM_WINDOWS_ONLY(x) x
#else
#define ENV_PLATFORM_WINDOWS_ONLY(x)
#endif


#ifdef ENV_PLATFORM_ORBIS
#define ENV_PLATFORM_ORBIS_ONLY(x) x
#else
#define ENV_PLATFORM_ORBIS_ONLY(x)
#endif

#ifdef FINAL
#define ENV_FINAL_CODE(x) x
#else
#define ENV_FINAL_CODE(x)
#endif

#ifdef FINAL
#define ENV_NOT_FINAL_CODE(x)
#else
#define ENV_NOT_FINAL_CODE(x) x;
#endif

#if defined(FINAL) | defined(RELEASE)
#define ENV_FINAL_OR_RELEASE_CODE(x) x
#else
#define ENV_FINAL_OR_RELEASE_CODE(x)
#endif

#ifdef FINAL
#define ENV_FINAL
#endif

#ifdef PROFILE
#define ENV_PROFILE_CODE(x) x
#else
#define ENV_PROFILE_CODE(x)
#endif

#ifdef PROFILE
#define ENV_PROFILE
#endif

// #ifdef RELEASE
// #define ENV_RELEASE
// #endif

namespace env
{

#ifndef ENV_ALIGN_16
#define ENV_DECL_ALIGN_16 __declspec(align(16))
#endif // ENV_ALIGN_16

#ifndef ENV_DEFINE_NEW_ALIGNED_16
#define ENV_DEFINE_NEW_ALIGNED_16 void* operator new(size_t i) { return _mm_malloc(i, 16); }
#endif // ENV_DEFINE_NEW_ALIGNED_16

#ifndef ENV_DEFINE_DELETE_ALIGNED_16
#define ENV_DEFINE_DELETE_ALIGNED_16 void operator delete(void* p) { _mm_free(p); } void operator delete(void* p, size_t t) { _mm_free(p); }
#endif // ENV_DEFINE_DELETE_ALIGNED_16

#ifndef ENV_DEFINE_ALLOC_ALIGNED_16
#define ENV_DEFINE_ALLOC_ALIGNED_16 \
        ENV_DEFINE_NEW_ALIGNED_16 \
        ENV_DEFINE_DELETE_ALIGNED_16 \
        ;
#endif // ENV_DEFINE_ALLOC_ALIGNED_16


#ifndef ENV_DELETE
#define ENV_DELETE(p) { if(p) { delete (p); (p) = nullptr; }}
#endif

#ifndef ENV_DELETE_ARRAY
#define ENV_DELETE_ARRAY(p) { if( p ) { delete[] (p); (p) = nullptr; }}
#endif 

#ifndef ENV_RELEASE
#define ENV_RELEASE(p) { if( p ) { p->Release(); (p) = nullptr; }}
#endif


//-----------------------------------------------------------------------------------
#if defined(_DEBUG) | defined(DEBUG)
//-----------------------------------------------------------------------------------

#define ENV_FORCEINLINE

#ifndef ENV_DBG_CODE
#define ENV_DBG_CODE(x) x;
#endif

#ifndef ENV_REL_CODE
#define ENV_REL_CODE(x) x;
#endif

#ifndef ENV_SCOPED_PROFILING
#define ENV_SCOPED_PROFILING(ProfilerClass, ProfilingGroup, ProfilingName)
#endif // ENV_SCOPED_PROFILING

#ifndef ENV_SCOPED_NAMED_ACCUM_CPU_PROFILING
#define ENV_SCOPED_NAMED_ACCUM_CPU_PROFILING(ProfilingGroup, ProfilingName) ENV_SCOPED_PROFILING(CCPUAccumProfiler, ProfilingGroup, ProfilingName)
#endif // ENV_SCOPED_NAMED_ACCUM_CPU_PROFILING

#ifndef ENV_SCOPED_NAMED_CPU_PROFILING
#define ENV_SCOPED_NAMED_CPU_PROFILING(ProfilingGroup, ProfilingName) ENV_SCOPED_PROFILING(CCPUProfiler, ProfilingGroup, ProfilingName)
#endif // ENV_SCOPED_NAMED_CPU_PROFILING

#ifndef ENV_SCOPED_CPU_PROFILING
#define ENV_SCOPED_CPU_PROFILING(ProfilingGroup, ProfilingName) ENV_SCOPED_NAMED_CPU_PROFILING(ProfilingGroup, #ProfilingName)
#endif // ENV_SCOPED_CPU_PROFILING

#ifndef ENV_SCOPED_ACCUM_CPU_PROFILING
#define ENV_SCOPED_ACCUM_CPU_PROFILING(ProfilingGroup, ProfilingName) ENV_SCOPED_PROFILING(CCPUAccumProfiler, ProfilingGroup, #ProfilingName)
#endif // ENV_SCOPED_ACCUM_CPU_PROFILING

#ifndef ENV_SCOPED_NAMED_GPU_PROFILING
#define ENV_SCOPED_NAMED_GPU_PROFILING(ProfilingGroup, ProfilingName) ENV_SCOPED_PROFILING(CGPUProfiler, ProfilingGroup, ProfilingName)
#endif // ENV_SCOPED_NAMED_GPU_PROFILING

#ifndef ENV_SCOPED_GPU_PROFILING
#define ENV_SCOPED_GPU_PROFILING(ProfilingGroup, ProfilingName) ENV_SCOPED_NAMED_GPU_PROFILING(ProfilingGroup, #ProfilingName)
#endif // ENV_SCOPED_GPU_PROFILING


#ifndef ENV_DEFINE_PROFILER
#define ENV_DEFINE_PROFILER(ProfilerClass, ProfilingGroup)
#endif // ENV_DEFINE_PROFILER

#ifndef ENV_DEFINE_CPU_PROFILER
#define ENV_DEFINE_CPU_PROFILER(ProfilingGroup) ENV_DEFINE_PROFILER(CCPUProfiler, ProfilingGroup)
#endif // ENV_DEFINE_CPU_PROFILER

#ifndef ENV_DEFINE_GPU_PROFILER
#define ENV_DEFINE_GPU_PROFILER(ProfilingGroup) ENV_DEFINE_PROFILER(CGPUProfiler, ProfilingGroup)
#endif // ENV_DEFINE_GPU_PROFILER

#ifndef ENV_BEGIN_PROFILING
#define ENV_BEGIN_PROFILING(ProfilingGroup, ProfilingName)
#endif // ENV_BEGIN_PROFILING

#ifndef ENV_END_PROFILING
#define ENV_END_PROFILING(ProfilingGroup, ProfilingName)
#endif // ENV_END_PROFILING

     /*! This macro resolves to ''(nothing) in release build. */
#ifndef ENV_ASSERT_DBG
#define ENV_ASSERT_DBG(x) { assert((x)); }
#endif

    /*! This macro is not included in release */
#ifndef ENV_DXCALL
#define ENV_DXCALL(x) { ENV_ASSERT_DBG(SUCCEEDED(x)); }
#endif

    /*! This macro resolves to a void return, if FAILED(x). */
#ifndef ENV_DXCALL_DBG
#define ENV_DXCALL_DBG(x) { ENV_ASSERT_DBG(SUCCEEDED(x)); }
#endif

    /*! This macro resolves to a false return, if FAILED(x). */
#ifndef ENV_DXCALL_RFALSE
#define ENV_DXCALL_RFALSE(x) \
    { \
        HRESULT h = x; \
        const char* pErr = 0; \
        if(FAILED(h)) \
        { \
            ENV_LOG_ERROR(std::string(pErr)); \
            return false; \
        } \
    }
#endif

//-----------------------------------------------------------------------------------
#elif defined(_FINAL) | defined(FINAL)
//-----------------------------------------------------------------------------------

#define ENV_FORCEINLINE __forceinline

#ifndef ENV_SCOPED_PROFILING
#define ENV_SCOPED_PROFILING(ProfilerClass, ProfilingGroup, ProfilingName)
#endif // ENV_SCOPED_PROFILING

#ifndef ENV_SCOPED_NAMED_ACCUM_CPU_PROFILING
#define ENV_SCOPED_NAMED_ACCUM_CPU_PROFILING(ProfilingGroup, ProfilingName) ENV_SCOPED_PROFILING(CCPUAccumProfiler, ProfilingGroup, ProfilingName)
#endif // ENV_SCOPED_NAMED_ACCUM_CPU_PROFILING

#ifndef ENV_SCOPED_NAMED_CPU_PROFILING
#define ENV_SCOPED_NAMED_CPU_PROFILING(ProfilingGroup, ProfilingName) ENV_SCOPED_PROFILING(CCPUStackedProfiler, ProfilingGroup, ProfilingName)
#endif // ENV_SCOPED_NAMED_CPU_PROFILING

#ifndef ENV_SCOPED_CPU_PROFILING
#define ENV_SCOPED_CPU_PROFILING(ProfilingGroup, ProfilingName) ENV_SCOPED_NAMED_CPU_PROFILING(ProfilingGroup, #ProfilingName)
#endif // ENV_SCOPED_CPU_PROFILING

#ifndef ENV_SCOPED_ACCUM_CPU_PROFILING
#define ENV_SCOPED_ACCUM_CPU_PROFILING(ProfilingGroup, ProfilingName) ENV_SCOPED_PROFILING(CCPUAccumProfiler, ProfilingGroup, #ProfilingName)
#endif // ENV_SCOPED_ACCUM_CPU_PROFILING

#ifndef ENV_SCOPED_NAMED_GPU_PROFILING
#define ENV_SCOPED_NAMED_GPU_PROFILING(ProfilingGroup, ProfilingName) ENV_SCOPED_PROFILING(CGPUProfiler, ProfilingGroup, ProfilingName)
#endif // ENV_SCOPED_NAMED_GPU_PROFILING

#ifndef ENV_SCOPED_GPU_PROFILING
#define ENV_SCOPED_GPU_PROFILING(ProfilingGroup, ProfilingName) ENV_SCOPED_NAMED_GPU_PROFILING(ProfilingGroup, #ProfilingName)
#endif // ENV_SCOPED_GPU_PROFILING

#ifndef ENV_DEFINE_PROFILER
#define ENV_DEFINE_PROFILER(ProfilerClass, ProfilingGroup)
#endif // ENV_DEFINE_PROFILER

#ifndef ENV_DEFINE_CPU_PROFILER
#define ENV_DEFINE_CPU_PROFILER(ProfilingGroup) ENV_DEFINE_PROFILER(CCPUStackedProfiler, ProfilingGroup)
#endif // ENV_DEFINE_CPU_PROFILER

#ifndef ENV_DEFINE_GPU_PROFILER
#define ENV_DEFINE_GPU_PROFILER(ProfilingGroup) ENV_DEFINE_PROFILER(CGPUProfiler, ProfilingGroup)
#endif // ENV_DEFINE_GPU_PROFILER

#ifndef ENV_BEGIN_PROFILING
#define ENV_BEGIN_PROFILING(ProfilingGroup, ProfilingName)
#endif // ENV_BEGIN_PROFILING

#ifndef ENV_END_PROFILING
#define ENV_END_PROFILING(ProfilingGroup, ProfilingName)
#endif // ENV_END_PROFILING


#ifndef ENV_DBG_CODE
#define ENV_DBG_CODE(x) {}
#endif

#ifndef ENV_REL_CODE
#define ENV_REL_CODE(x) x;
#endif

#ifndef ENV_ASSERT_DBG
#define ENV_ASSERT_DBG(x) {}
#endif

#ifndef ENV_DXCALL
#define ENV_DXCALL(x) (x);
#endif

#ifndef ENV_DXCALL_DBG
#define ENV_DXCALL_DBG(x) (x);
#endif

#ifndef ENV_DXCALL_RFALSE
#define ENV_DXCALL_RFALSE(x) (x)
#endif


//-----------------------------------------------------------------------------------
#elif defined(_RELEASE) || defined(RELEASE)
//-----------------------------------------------------------------------------------

#define ENV_FORCEINLINE __forceinline

#ifndef ENV_SCOPED_PROFILING
#define ENV_SCOPED_PROFILING(ProfilerClass, ProfilingGroup, ProfilingName) \
            ProfilerClass ProfilingGroup(#ProfilingGroup); if(ProfilingGroup.IsProfiling()) ProfilingGroup.Begin(ProfilingName);
#endif // ENV_SCOPED_PROFILING

#ifndef ENV_SCOPED_NAMED_ACCUM_CPU_PROFILING
#define ENV_SCOPED_NAMED_ACCUM_CPU_PROFILING(ProfilingGroup, ProfilingName) ENV_SCOPED_PROFILING(CCPUAccumProfiler, ProfilingGroup, ProfilingName)
#endif // ENV_SCOPED_NAMED_ACCUM_CPU_PROFILING

#ifndef ENV_SCOPED_NAMED_CPU_PROFILING
#define ENV_SCOPED_NAMED_CPU_PROFILING(ProfilingGroup, ProfilingName) ENV_SCOPED_PROFILING(CCPUStackedProfiler, ProfilingGroup, ProfilingName)
#endif // ENV_SCOPED_NAMED_CPU_PROFILING

#ifndef ENV_SCOPED_CPU_PROFILING
#define ENV_SCOPED_CPU_PROFILING(ProfilingGroup, ProfilingName) ENV_SCOPED_NAMED_CPU_PROFILING(ProfilingGroup, #ProfilingName)
#endif // ENV_SCOPED_CPU_PROFILING

#ifndef ENV_SCOPED_ACCUM_CPU_PROFILING
#define ENV_SCOPED_ACCUM_CPU_PROFILING(ProfilingGroup, ProfilingName) ENV_SCOPED_PROFILING(CCPUAccumProfiler, ProfilingGroup, #ProfilingName)
#endif // ENV_SCOPED_ACCUM_CPU_PROFILING

#ifndef ENV_SCOPED_NAMED_GPU_PROFILING
#define ENV_SCOPED_NAMED_GPU_PROFILING(ProfilingGroup, ProfilingName) ENV_SCOPED_PROFILING(CGPUProfiler, ProfilingGroup, ProfilingName)
#endif // ENV_SCOPED_NAMED_GPU_PROFILING

#ifndef ENV_SCOPED_GPU_PROFILING
#define ENV_SCOPED_GPU_PROFILING(ProfilingGroup, ProfilingName) ENV_SCOPED_NAMED_GPU_PROFILING(ProfilingGroup, #ProfilingName)
#endif // ENV_SCOPED_GPU_PROFILING


#ifndef ENV_DEFINE_PROFILER
#define ENV_DEFINE_PROFILER(ProfilerClass, ProfilingGroup) \
            ProfilerClass ProfilingGroup(#ProfilingGroup);
#endif // ENV_DEFINE_PROFILER

#ifndef ENV_DEFINE_CPU_PROFILER
#define ENV_DEFINE_CPU_PROFILER(ProfilingGroup) ENV_DEFINE_PROFILER(CCPUStackedProfiler, ProfilingGroup)
#endif // ENV_DEFINE_CPU_PROFILER

#ifndef ENV_DEFINE_GPU_PROFILER
#define ENV_DEFINE_GPU_PROFILER(ProfilingGroup) ENV_DEFINE_PROFILER(CGPUProfiler, ProfilingGroup)
#endif // ENV_DEFINE_GPU_PROFILER

#ifndef ENV_BEGIN_PROFILING
#define ENV_BEGIN_PROFILING(ProfilingGroup, ProfilingName) \
            if(ProfilingGroup.IsProfiling()) ProfilingGroup.Begin(#ProfilingName);
#endif // ENV_BEGIN_PROFILING

#ifndef ENV_END_PROFILING
#define ENV_END_PROFILING(ProfilingGroup, ProfilingName) \
            if(ProfilingGroup.IsProfiling()) ProfilingGroup.End(#ProfilingName);
#endif // ENV_END_PROFILING


#ifndef ENV_DBG_CODE
#define ENV_DBG_CODE(x) {}
#endif

#ifndef ENV_REL_CODE
#define ENV_REL_CODE(x) x;
#endif

#ifndef ENV_ASSERT_DBG
#define ENV_ASSERT_DBG(x) {}
#endif

#ifndef ENV_DXCALL
#define ENV_DXCALL(x) (x);
#endif

#ifndef ENV_DXCALL_DBG
#define ENV_DXCALL_DBG(x) (x);
#endif

#ifndef ENV_DXCALL_RFALSE
#define ENV_DXCALL_RFALSE(x) { if(FAILED(x)) { return false; }}
#endif

#endif


// #define COLOR_ARGB32(a,r,g,b) \
//     ((unsigned long)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
// #define COLOR_RGBA32(r,g,b,a) COLOR_ARGB32(a,r,g,b)
// #define COLOR_XRGB32(r,g,b)   COLOR_ARGB32(0xff,r,g,b)

} // env
#endif // MACROS_H