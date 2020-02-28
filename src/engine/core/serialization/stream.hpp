#pragma once

namespace quantum
{

/**
 * Defines a stream for reading and writing a sequence of bytes.
 *
 * Use FileSystem::open() to create a stream from file.
 */
class Stream
{
public:
    virtual ~Stream() { }

    /**
     * Returns true if this stream can perform read operations.
     */
    virtual bool CanRead() = 0;

    /**
     * Returns true if this stream can perform write operations.
     */
    virtual bool CanWrite() = 0;

    /**
     * Determins if this stream can seek.
	*/
    virtual bool CanSeek() = 0;

    /**
     * Closes this stream.
     */
    virtual void Close() = 0;

    /**
     * Reads an array of <code>count</code> elements, each of size <code>size</code>.
     */
    virtual size_t Read(void* ptr, size_t size, size_t count) = 0;

    /**
     * Reads a line from the stream.
     */
    virtual char* ReadLine(char* str, int num) = 0;

    /**
     * Writes an array of *count* elements, each of size *size*.
     */
    virtual size_t Write(const void* ptr, size_t size, size_t count) = 0;

    /**
     * Determines if the end of the stream has been reached.
     */
    virtual bool Eof() = 0;

    /**
     * Gets the length of the stream in bytes.
     */
    virtual size_t Length() = 0;

    /**
     * Returns the position of the file pointer. Zero is the start of the stream.
     */
    virtual long int Position() = 0;

    /**
     * Sets the position of the file pointer.
     */
    virtual bool Seek(long int offset, int origin) = 0;

    /**
     * Moves the file pointer to the start of the file.
     */
    virtual bool Rewind() = 0;

protected:

    Stream() { };

private:

    Stream(const Stream&);
};

}
