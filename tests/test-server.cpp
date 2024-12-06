#include "../include/Server.h"
#include <iostream>

// void HandleGet(const httplib::Request &req, httplib::Response &rsp) {
//     rsp.set_content(R"(<!DOCTYPE html>
//                         <html>
//                         <head>
//                         <meta charset="UTF-8">
//                             <title>Simple Form</title>
//                         </head>
//                         <body>
//                             <form action="/video" method="POST" enctype="multipart/form-data">
//                                 <label>视频名:</label>
//                                 <input type="text" name="name" required>
//                                 <br>
//                                 <label></label>
//                                 <label>描述:</label>
//                                 <input type="text" name="description" required>
//                                 <br>
//                                 <label>类型:</label>
//                                 <input type="text" name="type" required>
//                                 <br>
//                                 <label>视频:</label>
//                                 <input type="file" name="video" accept="video/*" required>
//                                 <br>
//                                 <label>封面:</label>
//                                 <input type="file" name="cover" accept="image/*" required>
//                                 <br>
//                                 <button type="submit">Submit</button>
//                             </form>
//                         </body>
//                         </html>
//                         )", "text/html");
// }


int main() 
{
    spdlog::set_level(spdlog::level::debug);
    Server srv(8888);
    srv.Start();
    
    return 0;
}

