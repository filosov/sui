#include "scstoragestream.hpp"
#include <QString>

ScStorageStream::ScStorageStream(QIODevice *dev, _sc_stream *stream, QObject *parent) :
    QIODevice(parent),
    mStream(stream)
{
    if (dev)
    {
        dev->open(QIODevice::ReadOnly);
        QByteArray data = dev->readAll();
        mStream = sc_stream_memory_new(data.data(), data.size(), SC_STREAM_READ, SC_FALSE);
        dev->close();
    }
}

ScStorageStream::~ScStorageStream()
{
    close();
}

bool ScStorageStream::open(QIODevice::OpenMode mode)
{
    mErrorString.clear();
    if (mStream)
    {
        mErrorString = "Stream is already opened";
        return false;
    }
    mStream = sc_stream_memory_new(NULL, 0, SC_STREAM_READ | SC_STREAM_READ, SC_FALSE);
    mStream->handler = this;
    mFlags = mode;
    return true;
}

void ScStorageStream::close()
{
    if(mStream)
    {
        mStream->free_func(mStream);
        mStream = NULL;
    }
}

bool ScStorageStream::seek(qint64 pos)
{
    mErrorString.clear();
    if (!mStream)
    {
        mErrorString = "There is no opened stream";
        return false;
    }
    else if (mStream->seek_func(mStream, SC_STREAM_SEEK_SET, pos) == SC_OK)
        return true;
    else
    {
        mErrorString = "Error during seek function execution";
        return false;
    }
}

qint64 ScStorageStream::pos() const
{
    if (!mStream)
        return -1;
    sc_uint32 resPos = 0;
    if (mStream->tell_func(mStream, &resPos) == SC_OK)
        return resPos;
    return -1;
}

bool ScStorageStream::atEnd() const
{
    if (!mStream)
        return false;
    return mStream->eof_func(mStream);
}

qint64 ScStorageStream::read(char *data, qint64 maxlen)
{
    mErrorString.clear();
    if (!mStream)
    {
        mErrorString = "Stream is not opened";
        return -1;
    }
    quint32 bytesRead = 0;
    mStream->read_func(mStream, data, maxlen, &bytesRead);
    return bytesRead;
}

qint64 ScStorageStream::readData(char *data, qint64 maxlen)
{
    return -1;
}

qint64 ScStorageStream::write(const char *data, qint64 len)
{
    mErrorString.clear();
    if (!mStream)
    {
        mErrorString = "Stream is not opened";
        return -1;
    }
    quint32 bytesWritten = 0;
    sc_char *copy = new sc_char[len];
    memcpy(copy, data, len);
    mStream->write_func(mStream, copy, len, &bytesWritten);
    delete[] copy;
    return bytesWritten;
}

qint64 ScStorageStream::writeData(const char *data, qint64 len)
{
    return -1;
}

QString ScStorageStream::errorString() const
{
    return mErrorString;
}

bool ScStorageStream::isOpen() const
{
    return mStream;
}

bool ScStorageStream::setStream(_sc_stream *stream)
{
    mErrorString.clear();
    if (mStream)
        close();
    mStream = stream;
    return true;
}

_sc_stream *ScStorageStream::scStream() const
{
    return mStream;
}
