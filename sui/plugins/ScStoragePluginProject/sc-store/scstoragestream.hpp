#ifndef SCSTORAGESTREAM_HPP
#define SCSTORAGESTREAM_HPP

#include <QIODevice>
#include "defines.hpp"
extern "C"{
#include <sc-store/sc_stream_private.h>
#include <sc-store/sc_stream_memory.h>
}

class SCSCTORESHARED_EXPORT ScStorageStream : public QIODevice
{
    Q_OBJECT
public:
    explicit ScStorageStream(QIODevice *dev = 0, _sc_stream *stream = NULL, QObject *parent = 0);
    virtual ~ScStorageStream();
    virtual bool open(OpenMode mode);
    virtual void close();
    virtual bool seek(qint64 pos);
    virtual qint64 pos() const;
    virtual bool atEnd() const;
    virtual qint64 read(char *data, qint64 maxlen);
    virtual qint64 write(const char *data, qint64 len);
    QString errorString() const;
    bool isOpen() const;
    bool setStream(_sc_stream *stream);
    _sc_stream* scStream() const;
protected:
    virtual qint64 readData(char *data, qint64 maxlen);
    virtual qint64 writeData(const char *data, qint64 len);
private:
    _sc_stream *mStream;
    QString mErrorString;
    OpenMode mFlags;
};

#endif // SCSTORAGESTREAM_HPP
