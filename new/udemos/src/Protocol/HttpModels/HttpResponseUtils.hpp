//
//  UdHttpResponseUtils.hpp
//  udemos
//
//  Created by Sinisa Abramovic on 17.03.2023..
//

#ifndef UdHttpResponseUtils_hpp
#define UdHttpResponseUtils_hpp

#include <zlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <map>

class HttpResponseUtils
{
private:
    /* data */
public:
    HttpResponseUtils(/* args */){};
    ~HttpResponseUtils(){};

    static std::string compressPayload(const std::string &data)
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
        std::string compressedData;

        do
        {
            zs.next_out = reinterpret_cast<Bytef *>(outbuffer);
            zs.avail_out = sizeof(outbuffer);

            ret = deflate(&zs, Z_FINISH);

            if (compressedData.size() < zs.total_out)
            {
                compressedData.append(outbuffer, zs.total_out - compressedData.size());
            }

        } while (ret == Z_OK);

        deflateEnd(&zs);

        if (ret != Z_STREAM_END)
        {
            return "";
        }

        return compressedData;
    }

    static std::map<std::string, std::string> corsHeadersDefault()
    {
        std::map<std::string, std::string> corsHeaders;
        corsHeaders["Access-Control-Allow-Origin"] = "*";
        corsHeaders["Access-Control-Allow-Methods"] = "GET, POST, PUT, DELETE, OPTIONS";
        corsHeaders["Access-Control-Allow-Headers"] = "Content-Type, Authorization";

        return corsHeaders;
    }

};

#endif

