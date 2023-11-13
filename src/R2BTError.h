#ifndef __R2BTError_H__
#define __R2BTError_H__

struct R2BTError
{
    enum Type {
        InvalidJSONReceived = -1
    };

    R2BTError(const R2BTError::Type code, const char *message) : code(code), message(message)
    {
    }
    const int code;
    const char *message;
};

#endif