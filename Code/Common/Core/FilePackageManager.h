/*
* ============================================================
* Copyright (C) Christian Hamm. All Rights Reserved.
* ============================================================
*/

#pragma once
#ifndef FILE_PACKAGE_MANAGER_H
#define FILE_PACKAGE_MANAGER_H

#include <Common/Common.h>
#include <Common/Core/Filesystem.h>

//#include <Common/Serialize.h>
#include "boost/filesystem.hpp"

//#define ENV_USE_FILE_PACKAGES
//#define ENV_CREATE_FILE_PACKAGES

namespace env
{
    class CFilePackageManager
    {
    public:
        friend class CSingletonStatic<CFilePackageManager>;

        struct FileInfo
        {
            FileInfo()
                : dataStart(0)
                , dataLength(0) {}

            unsigned long dataStart;
            unsigned long dataLength;

            template< class Archive > void serialize(Archive& ar, const unsigned int version)
            {
                ar & dataStart;
                ar & dataLength;
            }
        };

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & m_fileInfos;
        }

        std::map<std::string, FileInfo> m_fileInfos;
        std::vector<char> m_dataBuffer;
        std::ifstream m_ifstream;

    public:

        void SavePackage(const std::string& strFilename)
        {
            SavePackageInfo(strFilename + "Info");
            SavePackageData(strFilename + "Data");
        }

        void SavePackageData(const std::string& strFilename)
        {
            std::ofstream ofs(strFilename, std::ios::binary);
            ofs.write(&m_dataBuffer[0], m_dataBuffer.size());

        }

        void SavePackageInfo(const std::string& strFilename)
        {
            std::ofstream ofs(strFilename, std::ios::binary);
            boost::archive::binary_oarchive oa(ofs);

            oa & *this;
        }

        void LoadPackage(const std::string& strFilename)
        {
            m_ifstream = std::ifstream(strFilename + "Data", std::ios::binary);

            std::ifstream ifs(strFilename + "Info", std::ios::binary);
            boost::archive::binary_iarchive ia(ifs);
 
            ia & *this;
        }

        void LoadFile(std::string strFilename, std::vector<char>& data)
        {
            ReplaceBackwardSlashes(strFilename);

            auto it = m_fileInfos.find(strFilename);

            if (it != m_fileInfos.end())
            {
                data.resize(it->second.dataLength);

                m_ifstream.seekg(0, m_ifstream.beg);
                m_ifstream.seekg(it->second.dataStart, m_ifstream.beg);
                ENV_ASSERT_DBG(m_ifstream.good());
                m_ifstream.read(&data[0], data.size());
                ENV_ASSERT_DBG(m_ifstream.good());
            }
        }

        void GatherFiles(const std::string& strDirectory, unsigned long& dataOffset)
        {
            boost::filesystem::path path;
            boost::filesystem::directory_iterator end_itr; // default construction yields past-the-end

            for (boost::filesystem::directory_iterator itr(strDirectory); itr != end_itr; ++itr)
            {
                if (boost::filesystem::is_directory(itr->status()))
                {
                    GatherFiles(itr->path().string(), dataOffset);
                }
                else //if (itr->leaf() == file_name) // see below
                {
                    std::string strFilename = itr->path().string();

                    ReplaceBackwardSlashes(strFilename);

                    unsigned int fileSize = boost::filesystem::file_size(strFilename);

                    FileInfo fileInfo;
                    fileInfo.dataLength = fileSize;
                    fileInfo.dataStart = dataOffset;

                    m_fileInfos[strFilename] = fileInfo;

                    dataOffset += fileSize;
                }
            }
        }

        void CreateFilePackage(const std::string& strDirectory, const std::string& strPackageName)
        {
            unsigned long dataOffset = 0;

            std::remove((strDirectory + "/" + strPackageName + "Info").c_str());
            std::remove((strDirectory + "/" + strPackageName + "Data").c_str());

            GatherFiles(strDirectory, dataOffset);

            m_dataBuffer.resize(dataOffset);

            for (auto it = m_fileInfos.begin(); it != m_fileInfos.end(); ++it)
            {
                std::string strFileName = it->first;
                std::ifstream ifs(strFileName, std::ifstream::binary);

                if (ifs.is_open())
                {
                    ifs.read(&m_dataBuffer[it->second.dataStart], it->second.dataLength);
                    ENV_ASSERT_DBG(ifs.good());
                }
            }

            SavePackage(strDirectory + "/" + strPackageName);

//             std::vector<char> data;
// 
//             auto it = m_fileInfos.find("../Resources/AudioTest/10 Menue.wav");
// 
//             if (it != m_fileInfos.end())
//             {
//                 data.resize(it->second.dataLength);
//                 memcpy(&data[0], &m_dataBuffer[it->second.dataStart], it->second.dataLength);
//             }
// 
//             LoadPackage("../Resources/AudioTest/Audio");
//             LoadFile("../Resources/AudioTest/10 Menue.wav", data);
        }
    };

    typedef CSingletonStatic<CFilePackageManager> CSingletonFilePackageMgr;

    extern CFilePackageManager* gSoundPackageMgr;
    extern CFilePackageManager* gTexturePackageMgr;

#define AUDIO_PACKAGE_NAME "../Resources/Audio/Audio"
#define TEXTURE_PACKAGE_NAME "../Resources/Textures/Texture"

} // env 
#endif // FILE_PACKAGE_MANAGER_H