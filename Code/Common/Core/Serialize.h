/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once


//#define ENV_SERIALIZATION_SAVE_TEXT_ARCHIVE
//#define ENV_SERIALIZATION_LOAD_TEXT_ARCHIVE

#define ENV_SERIALIZATION_SAVE_BINARY_ARCHIVE
#define ENV_SERIALIZATION_LOAD_BINARY_ARCHIVE

#include <Common/Common.h>



#ifdef ENV_PLATFORM_WINDOWS

namespace boost
{
    inline void throw_exception(std::exception const & e) {}
}

#ifdef ENV_SERIALIZATION_LOAD_TEXT_ARCHIVE
#include <boost/archive/text_iarchive.hpp>
#endif
#ifdef ENV_SERIALIZATION_SAVE_TEXT_ARCHIVE
#include <boost/archive/text_oarchive.hpp>
#endif

#ifdef ENV_SERIALIZATION_LOAD_BINARY_ARCHIVE
#include <boost/archive/binary_iarchive.hpp>
#endif
#ifdef ENV_SERIALIZATION_SAVE_BINARY_ARCHIVE
#include <boost/archive/binary_oarchive.hpp>
#endif

#include "boost/serialization/vector.hpp"
#include "boost/serialization/map.hpp"
#include "boost/serialization/list.hpp"
#include <boost/serialization/set.hpp>
#include <boost/serialization/export.hpp>


#include <boost/config.hpp>
#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std
{ 
    using ::remove;
}
#endif

#include <boost/archive/tmpdir.hpp>
#include <boost/serialization/split_free.hpp>

#endif // ENV_PLATFORM_WINDOWS

#ifdef ENV_PLATFORM_ORBIS
#define BOOST_SERIALIZATION_SPLIT_MEMBER()                       \
template<class Archive>                                          \
void serialize(                                                  \
    Archive &ar,                                                 \
    const unsigned int file_version                              \
){                                                               \
}    
#endif // ENV_PLATFORM_ORBIS

namespace boost
{
#ifdef ENV_PLATFORM_ORBIS
    struct RECT
    {
        long left;
        long top;
        long right;
        long bottom;
    };
#endif // ENV_PLATFORM_ORBIS

    namespace serialization
    {
#ifdef ENV_PLATFORM_ORBIS
        class access {};

        template<class Base, class Derived>
        Derived& base_object(Derived &d) { return d; }

        template<class Archive, class T>
        inline void split_free(Archive & ar, T & t, const unsigned int file_version) {}
#endif // ENV_PLATFORM_ORBIS

        //-----------------------------------------------------------------------------------
        //  Boost::Serialization support for: RECT
        //
        template<class Archive>
        inline void save(Archive & ar, const RECT& t, const unsigned int file_version)
        {
            ar << t.bottom;
            ar << t.left;
            ar << t.right;
            ar << t.top;
        }

        template<class Archive>
        inline void load(Archive & ar, RECT& t, const unsigned int file_version)
        {
            ar >> t.bottom;
            ar >> t.left;
            ar >> t.right;
            ar >> t.top;
        }

        template<class Archive>
        inline void serialize(Archive & ar, RECT& t, const unsigned int file_version)
        {
            boost::serialization::split_free(ar, t, file_version);
        }


        //-----------------------------------------------------------------------------------
        //  Boost::Serialization support for: VECTOR2
        //
        template<class Archive>
        inline void save(Archive & ar, const env::Vec2& t, const unsigned int file_version)
        {
            ar << t.x;
            ar << t.y;
        }

        template<class Archive>
        inline void load(Archive & ar, env::Vec2& t, const unsigned int file_version)
        {
            ar >> t.x;
            ar >> t.y;
        }

        template<class Archive>
        inline void serialize(Archive & ar, env::Vec2& t, const unsigned int file_version)
        {
            boost::serialization::split_free(ar, t, file_version);
        }

        //-----------------------------------------------------------------------------------
        //  Boost::Serialization support for: VECTOR3
        //
        template<class Archive>
        inline void save(Archive & ar, const env::Vec3& t, const unsigned int file_version)
        {
            ar << t.x;
            ar << t.y;
            ar << t.z;
        }

        template<class Archive>
        inline void load(Archive & ar, env::Vec3& t, const unsigned int file_version)
        {
            ar >> t.x;
            ar >> t.y;
            ar >> t.z;
        }

        template<class Archive>
        inline void serialize(Archive & ar, env::Vec3& t, const unsigned int file_version)
        {
            boost::serialization::split_free(ar, t, file_version);
        }

        //-----------------------------------------------------------------------------------
        //  Boost::Serialization support for: VECTOR4
        //
        template<class Archive>
        inline void save(Archive & ar, const env::Vec4& t, const unsigned int file_version)
        {
            ar << t.x;
            ar << t.y;
            ar << t.z;
            ar << t.w;
        }

        template<class Archive>
        inline void load(Archive & ar, env::Vec4& t, const unsigned int file_version)
        {
            ar >> t.x;
            ar >> t.y;
            ar >> t.z;
            ar >> t.w;
        }

        template<class Archive>
        inline void serialize(Archive & ar, env::Vec4& t, const unsigned int file_version)
        {
            boost::serialization::split_free(ar, t, file_version);
        }

        //-----------------------------------------------------------------------------------
        //  Boost::Serialization support for: MATRIX
        //
        template<class Archive>
        inline void save(Archive & ar, const env::Matrix4x4& t, const unsigned int file_version)
        {
#ifdef ENV_PLATFORM_WINDOWS
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    ar << t(i, j);
#endif // ENV_PLATFORM_WINDOWS
        }

        template<class Archive>
        inline void load(Archive & ar, env::Matrix4x4& t, const unsigned int file_version)
        {
#ifdef ENV_PLATFORM_WINDOWS
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    ar >> t(i, j);
#endif // ENV_PLATFORM_WINDOWS
        }

        template<class Archive>
        inline void serialize(Archive & ar, env::Matrix4x4& t, const unsigned int file_version)
        {
            boost::serialization::split_free(ar, t, file_version);
        }

        //-----------------------------------------------------------------------------------
        //  Boost::Serialization support for: QUATERNION
        //
        template<class Archive>
        inline void save(Archive & ar, const env::Quaternion& t, const unsigned int file_version)
        {
#ifdef ENV_PLATFORM_WINDOWS
            ar << t.x;
            ar << t.y;
            ar << t.z;
            ar << t.w;
#endif // ENV_PLATFORM_WINDOWS
        }

        template<class Archive>
        inline void load(Archive & ar, env::Quaternion& t, const unsigned int file_version)
        {
#ifdef ENV_PLATFORM_WINDOWS
            ar >> t.x;
            ar >> t.y;
            ar >> t.z;
            ar >> t.w;
#endif // ENV_PLATFORM_WINDOWS
        }

        template<class Archive>
        inline void serialize(Archive & ar, env::Quaternion& t, const unsigned int file_version)
        {
            boost::serialization::split_free(ar, t, file_version);
        }

    } // serialization
} // boost
