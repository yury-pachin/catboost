#pragma once
#include "bin_saver.h"

#include <util/stream/input.h>
#include <util/stream/output.h>
#include <util/stream/file.h>


class TYaStreamInput : public IBinaryStream
{
    TInputStream &Stream;

    int WriteImpl(const void*, int) override
    {
        Y_ASSERT(0);
        return 0;
    }
    int ReadImpl(void *userBuffer, int size) override
    {
        return (int)Stream.Read(userBuffer, (size_t) size);
    }
    bool IsValid() const override { return true; }
    bool IsFailed() const override { return false; }
public:
    TYaStreamInput(TInputStream &stream)
        : Stream(stream) {
    }
};

template<class T>
inline void SerializeFromStream(TInputStream &stream, T &c)
{
    TYaStreamInput f(stream);
    {
        IBinSaver bs(f, true);
        bs.Add(1, &c);
    }
}

template<class T>
inline void SerializeFromFile(const TString &fileName, T &c)
{
    TIFStream in(fileName);
    SerializeFromStream(in, c);
}


class TYaStreamOutput : public IBinaryStream
{
    TOutputStream &Stream;

    int WriteImpl(const void* what, int size) override
    {
        Stream.Write(what, (size_t) size);
        return size;
    }
    int ReadImpl(void*, int) override
    {
        Y_ASSERT(0);
        return 0;
    }
    bool IsValid() const override { return true; }
    bool IsFailed() const override { return false; }
public:
    TYaStreamOutput(TOutputStream &stream)
        : Stream(stream) {
    }
};

template<class T>
inline void SerializeToStream(TOutputStream &stream, T &c)
{
    TYaStreamOutput f(stream);
    {
        IBinSaver bs(f, false);
        bs.Add(1, &c);
    }
}

template<class T>
inline void SerializeToFile(const TString &fileName, T &c)
{
    TOFStream out(fileName);
    SerializeToStream(out, c);
}
