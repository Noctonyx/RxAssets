//
// Created by shane on 16/02/2021.
//

#ifndef RX_VFS_H
#define RX_VFS_H

#include <memory>
#include <filesystem>
#include <thread>
#include <unordered_map>
#include "miniz.h"

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <Pathcch.h>
#include <shlwapi.h>
#endif // WIN32

namespace RxAssets
{
    struct Mount
    {
        Mount(std::filesystem::path path, std::string mountPoint)
            : path(std::move(path))
            , mountPoint(std::move(mountPoint)) {}

        std::filesystem::path path;
        std::string mountPoint;
        bool isZip;
        mz_zip_archive zip;
    };

    struct CatalogEntry
    {
        std::string name;
        std::filesystem::path relativePath;
        uint32_t mountIndex;
        //uint32_t size;
        uint32_t zipIndex;
    };
    
    struct MonitorData
    {
        std::filesystem::path path;
        std::vector<BYTE> buffer{};
        DWORD bytes_returned = 0;
        OVERLAPPED overlapped_buffer{ 0, 0 , {{0, 0}},  0 };
        HANDLE directory{NULL};
    };

    class Vfs
    {
    public:

        Vfs() {}
        virtual ~Vfs() {}

        void addMount(const std::filesystem::path & path, const std::string & mountPoint);
        void scan();
        size_t getFilesize(const std::filesystem::path & path) const;
        size_t getFileContents(const std::filesystem::path & path, std::byte * data) const;
        bool assetExists(const std::filesystem::path & path) const;
        std::string getAssetAsString(const std::filesystem::path & path) const;
        void writeFile(const std::filesystem::path & path, std::byte * data, size_t size) const;

        void shutdown()
        {
            stopMonitor();
            delete instance_;
            instance_ = nullptr;
        }

        static Vfs * getInstance()
        {
            if (!instance_) {
                instance_ = new Vfs;
            }
            return instance_;
        }

        void monitorForChanges();
        bool hasChanged();

    protected:
        void stopMonitor();
        void clearCatalog();
        void loadDirectoryCatalog(const std::filesystem::path & path, const std::string & mount, uint32_t mountIndex);
        void loadZipCatalog(const std::filesystem::path & path, const std::string & mountPoint, uint32_t mountIndex, Mount & mount);
        CatalogEntry getCatalogEntry(const std::filesystem::path & path) const;
        static size_t getZipAsset(Mount & mount, uint32_t zipIndex, std::byte * data, size_t size);
        static size_t getFileAsset(const std::filesystem::path & path, std::byte * data);

    private:
        static Vfs * instance_ ;
        std::vector<Mount> mounts_;
        std::vector<CatalogEntry> catalog_;
        std::unordered_map<std::string, uint32_t> catalogIndex_;
        //bool writeable_;
        std::filesystem::path writeableDirectory_;

        std::vector<MonitorData> monData;
        std::thread monitorThread;
        HANDLE closeEvent;

        bool changed = false;
    };

    Vfs * vfs();
}
#endif //RX_VFS_H
