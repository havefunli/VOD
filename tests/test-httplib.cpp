#include "../include/httplib.h"
#include <iostream>

void HandleGet(const httplib::Request &req, httplib::Response &rsp) {
    rsp.set_content(R"(<!DOCTYPE html>
                        <html>
                        <head>
                        <meta charset="UTF-8">
                            <title>Simple Form</title>
                        </head>
                        <body>
                            <form action="/submit" method="POST" enctype="multipart/form-data">
                                <label>昵称:</label>
                                <input type="text" name="nickname" required>
                                <br>
                                <label>图片:</label>
                                <input type="file" name="image" accept="image/*" required>
                                <br>
                                <button type="submit">Submit</button>
                            </form>
                        </body>
                        </html>
                        )", "text/html");
}

void HandlePost(const httplib::Request &req, httplib::Response &rsp) 
{
    if (req.has_file("nickname")) 
    {
        httplib::MultipartFormData nickname = req.get_file_value("nickname");
        std::cout << nickname.content << std::endl;
        std::cout << nickname.content_type << std::endl;
        std::cout << nickname.filename << std::endl;
        std::cout << nickname.name << std::endl;
    }

    if (req.has_file("image")) 
    {
        auto avatar = req.get_file_value("image");
        // std::cout << avatar.content << std::endl;
        std::cout << avatar.content_type << std::endl;
        std::cout << avatar.filename << std::endl;
        std::cout << avatar.name << std::endl;
    }

    rsp.set_content("<h1>Form submitted successfully!</h1><p>Thank you A-A</p>", "text/html");
}

int main() {
    httplib::Server srv;

    // Routes
    srv.Get("/", HandleGet);               // Form page
    srv.Post("/submit", HandlePost);       // Form submission

    // Listen on port 8888
    std::cout << "Server is running at http://0.0.0.0:8888\n";
    srv.listen("0.0.0.0", 8888);

    return 0;
}

