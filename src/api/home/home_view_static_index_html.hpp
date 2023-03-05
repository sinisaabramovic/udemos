#ifndef home_view_static_index_html_hpp
#define home_view_static_index_html_hpp

#include <iostream>
#include <fstream>
#include <string>

class home_view_static_index_html
{
private:
    /* data */
public:
    home_view_static_index_html(/* args */);
    ~home_view_static_index_html();

    std::string read_from_html_file(std::string filename)
    {
        std::string html = "";
        std::ifstream file(filename);

        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                html += line + "\n";
            }
            file.close();
        }
        else
        {
            std::cerr << "Error: could not open file " << filename << std::endl;
        }

        return html;
    }

    std::string from_static_value() const
    {
        std::string html_str = R"(<!DOCTYPE html>
<html>
  <head>
    <title>udemos - index</title>
    <style>
      body {
        background-color: rgb(197, 221, 230);
      }

      .centered {
        margin: 0 auto;
        width: 60%;
        height: 40%;
        border: 5px solid rgb(208, 77, 77);
        text-align: center;
        display: flex;
        justify-content: center;
        align-items: center;
      }

      .bold {
        font-weight: bold;
        line-height: 1.4;
      }

      .light-grey {
        color: #888;
        border-top: 1px solid #ccc;
        padding-top: 10px;
      }

      table {
        margin: 0 auto;
        width: 80%;
        border-collapse: collapse;
      }

      td {
        border: none;
        text-align: center;
        padding: 5px;
      }

      td:first-child {
        font-weight: bold;
        width: 30%;
      }

      caption {
        font-weight: bold;
        font-size: 1.2em;
        margin-bottom: 10px;
      }
    </style>
  </head>

  <body>
    <div class="centered">
      <table>
        <caption>udemos</caption>
        <tr>
          <td>provides a basic implementation of an HTTP server that can listen for incoming connections and handle requests using a provided router. The class can be customized and extended to meet the needs of specific applications.</td>
        </tr>
        <tr>
          <td>
            <span class="light-grey">Sinisa Abramovic, 2023</span>
          </td>
        </tr>
      </table>
    </div>
  </body>
</html>)";
        return html_str;
    }
};

home_view_static_index_html::home_view_static_index_html(/* args */)
{
}

home_view_static_index_html::~home_view_static_index_html()
{
}

#endif