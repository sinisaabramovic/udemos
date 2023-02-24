#ifndef ud_http_response_utils_hpp
#define ud_http_response_utils_hpp

#include <zlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

class ud_http_response_utils
{
private:
    /* data */
public:
    ud_http_response_utils(/* args */){};
    ~ud_http_response_utils(){};

    static std::string compress_payload(const std::string &data)
    {
        z_stream zs;
        memset(&zs, 0, sizeof(zs));

        if (deflateInit2(&zs, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15 | 16, 8, Z_DEFAULT_STRATEGY) != Z_OK)
        {
            return "";
        }

        zs.next_in = (Bytef *)data.data();
        zs.avail_in = data.size();

        int ret;
        char outbuffer[32768];
        std::string compressed_data;

        do
        {
            zs.next_out = reinterpret_cast<Bytef *>(outbuffer);
            zs.avail_out = sizeof(outbuffer);

            ret = deflate(&zs, Z_FINISH);

            if (compressed_data.size() < zs.total_out)
            {
                compressed_data.append(outbuffer, zs.total_out - compressed_data.size());
            }

        } while (ret == Z_OK);

        deflateEnd(&zs);

        if (ret != Z_STREAM_END)
        {
            return "";
        }

        return compressed_data;
    }
};

#endif