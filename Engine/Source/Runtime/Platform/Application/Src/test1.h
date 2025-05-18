// #ifndef RT_TEST1_SCENE
// #define RT_TEST1_SCENE
// #include <Security/Security.h>
// #include <_types/_uint8_t.h>
// #include <sys/_types/_size_t.h>
// #include <cstddef>
// #include <iostream>

// #include "camera.h"
// #include "Core/Math/VectorInclude.h"
// #include "Platform/Application/Include/RTApplication.h"
// #include "Platform/Application/Src/geometry.h"
// #include "Core/HAL/System/Include/SystemInfo.h"
// #include "Core/HAL/Window/Include/IWindow.h"
// #include "Core/Math/IMath.h"

// #include "Function/RenderManager/Include/RenderManager.h"
// #include "Platform/ImageParser/Include/STBImageParser.h"
// #include "Platform/RHI/Include/CPU/SoftRenderer.h"
// #include "Core/HAL/Input/Include/InputEnums.h"
// #include "model.h"
// #include "tgaimage.h"

// namespace test1 {

// const int width  = 800;
// const int height = 600;
// const int depth  = 255;
// Model *model = nullptr;
// ReiToEngine::Camera* camera = nullptr;
// double *zbuffer = nullptr;

// Vec3f light_dir = Vec3f(1,-1,1).normalize();
// Vec3f eye(1,1,3);
// Vec3f center(0,0,0);

// Matrix viewport(int x, int y, int w, int h) {
//     Matrix m = Matrix::identity(4);
//     m[0][3] = x+w/2.f;
//     m[1][3] = y+h/2.f;
//     m[2][3] = depth/2.f;

//     m[0][0] = w/2.f;
//     m[1][1] = h/2.f;
//     m[2][2] = depth/2.f;
//     return m;
// }

// void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
//     bool steep = false;
//     if (std::abs(x0-x1)<std::abs(y0-y1)) {
//         std::swap(x0, y0);
//         std::swap(x1, y1);
//         steep = true;
//     }
//     if (x0>x1) {
//         std::swap(x0, x1);
//         std::swap(y0, y1);
//     }

//     for (int x=x0; x<=x1; x++) {
//         float t = (x-x0)/(float)(x1-x0);
//         int y = y0*(1.-t) + y1*t;
//         if (steep) {
//             image.set(y, x, color);
//         } else {
//             image.set(x, y, color);
//         }
//     }
// }

// Matrix lookat(Vec3f eye, Vec3f center, Vec3f up) {
//     Vec3f z = (eye-center).normalize();
//     Vec3f x = (up^z).normalize();
//     Vec3f y = (z^x).normalize();
//     Matrix res = Matrix::identity(4);
//     for (int i=0; i<3; i++) {
//         res[0][i] = x[i];
//         res[1][i] = y[i];
//         res[2][i] = z[i];
//         res[i][3] = -center[i];
//     }
//     return res;
// }

// void triangle(ReiToEngine::Vec4d t0, ReiToEngine::Vec4d t1, ReiToEngine::Vec4d t2, float ity0, float ity1, float ity2, TGAImage &image, double *zbuffer) {
//     if (t0.y==t1.y && t0.y==t2.y) return; // i dont care about degenerate triangles
//     if ((t0.x < 0 && t1.x < 0 && t2.x < 0)
//         || (t0.y < 0 && t1.y < 0 && t2.y < 0)
//         || (t0.x > test1::width && t1.x > test1::width && t2.x > test1::width)
//         || (t0.y > test1::height && t1.y > test1::height && t2.y > test1::height)
//     ) return; // i dont care about degenerate triangles
//     std::cout << t0 << std::endl;
//     std::cout << t1 << std::endl;
//     std::cout << t2 << std::endl;
//     int tempx, tempy,tempz,tempw;
//     if (t0.y>t1.y) {
//     std::swap(t0, t1);
//     std::swap(ity0, ity1);
//     }
//     if (t0.y>t2.y) {
//     std::swap(t0, t2);

//     std::swap(ity0, ity2);
//         }
//     if (t1.y>t2.y) {
//     std::swap(t1, t2);

//     std::swap(ity1, ity2);
//     }

//     int total_height = t2.y-t0.y;
//     for (int i=0; i< total_height; i++) {
//         bool second_half = i>t1.y-t0.y || t1.y==t0.y;
//         int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y;
//         double alpha = (double)i/total_height;
//         double beta  = (double)(i-(second_half ? t1.y-t0.y : 0))/segment_height; // be careful: with above conditions no division by zero here


//         ReiToEngine::Vec4d A    =               t0  + ReiToEngine::Vec4d(t2-t0)*alpha;

//         ReiToEngine::Vec4d B    = second_half ? t1  + ReiToEngine::Vec4d(t2-t1  )*beta : t0  + ReiToEngine::Vec4d(t1-t0  )*beta;
//         double ityA =               ity0 +   (ity2-ity0)*alpha;
//         double ityB = second_half ? ity1 +   (ity2-ity1)*beta : ity0 +   (ity1-ity0)*beta;
//         if (A.x>B.x) {
//             std::swap(A,B);
//             std::swap(ityA, ityB);
//             }

//         for (int j=A.x; j<=B.x; j++) {
//             double phi = B.x==A.x ? 1. : (double)(j-A.x)/(B.x-A.x);
//             ReiToEngine::Vec4d    P = ReiToEngine::Vec4d(A) +  ReiToEngine::Vec4d(B-A)*phi;
//             double ityP =    ityA  + (ityB-ityA)*phi;

//             int int_x, int_y;
//             int_x = P.x;
//             int_y = P.y;
//             int idx = int_x+int_y*width;
//             if (int_x>=test1::width||int_y>=test1::height||int_x<0||int_y<0)
//             {
//                 printf("x: %d, y: %d, z: %d\n", int_x, int_y, idx);
//                 continue;
//             }
//             if (P.z >= 0 && P.z<zbuffer[idx]) {
//                 zbuffer[idx] = P.z;
//                 image.set(int_x, int_y, TGAColor(255*ityP, 255*ityP, 255*ityP, 255));}
//         }
//     }
// }

// using namespace ReiToEngine;

// SoftRenderer* render;
// size_t SBO;
// size_t FBO;
// TGAImage* image;

// uint8_t* frame_buffer;

// const TGAColor white = TGAColor(255, 255, 255, 255);
// const TGAColor red   = TGAColor(255, 0,   0,   255);

// ::Vec3f world2screen(::Vec3f v) {
//     return ::Vec3f(int((v.x+1.)*width/2.+.5), int((v.y+1.)*height/2.+.5), v.z);
// }


// void InitTestSpace(SoftRenderer* ptr)
// {
//     camera = new Camera(Vec3d({0,0,-2}));
//     render = ptr;
//     // SBO = render->CreateSceneInfo();
//     model = new Model("../obj/african_head/african_head.obj");
//     zbuffer = new double[width*height];
//     frame_buffer = new uint8_t[width*height*4];
//     image = new TGAImage(width, height, TGAImage::RGBA);
// }

// void MoveCamera(const InputEvent& event)
// {
//     printf("MoveCamera: %hu\n", event.inputCode);
//     printf("MoveCamera: %c\n", event.codepoint);
//     printf("MoveCamera: %hu\n", ReiToEngine::EINPUT_KEY_CODE::INPUT_KEYBOARD_W);
//     printf("MoveCamera: %hu\n", ReiToEngine::EINPUT_KEY_CODE::INPUT_KEYBOARD_S);

//     if (event.inputCode == ReiToEngine::EINPUT_KEY_CODE::INPUT_KEYBOARD_W)
//     {
//         camera->ProcessKeyboard(ReiToEngine::Camera_Movement::FORWARD, 0.5);
//     }
//     else if (event.inputCode == ReiToEngine::EINPUT_KEY_CODE::INPUT_KEYBOARD_S)
//     {
//         camera->ProcessKeyboard(ReiToEngine::Camera_Movement::BACKWARD, 0.5);
//     }
//     else if (event.inputCode == ReiToEngine::EINPUT_KEY_CODE::INPUT_KEYBOARD_A)
//     {
//         camera->ProcessKeyboard(ReiToEngine::Camera_Movement::LEFT, 0.5);
//     }
//     else if (event.inputCode == ReiToEngine::EINPUT_KEY_CODE::INPUT_KEYBOARD_D)
//     {
//         camera->ProcessKeyboard(ReiToEngine::Camera_Movement::RIGHT, 0.5);
//     }
//     else if (event.inputCode == ReiToEngine::EINPUT_KEY_CODE::INPUT_KEYBOARD_C)
//     {
//         camera->ProcessKeyboard(ReiToEngine::Camera_Movement::UP, 0.5);
//     }
//     else if (event.inputCode == ReiToEngine::EINPUT_KEY_CODE::INPUT_KEYBOARD_Z)
//     {
//         camera->ProcessKeyboard(ReiToEngine::Camera_Movement::DOWN, 0.5);
//     }
// }

// float last_mouseX = 0;
// float last_mouseY = 0;

// void MoveCameraMouse(const InputEvent& event)
// {
//     printf("MoveCamera Mouse: %f\n", event.positionY);
//     printf("MoveCamera Mouse: %f\n", event.positionX);
//     float mouseX = event.positionX;
//     float mouseY = event.positionY;
//     camera->ProcessMouseMovement(mouseX - last_mouseX, mouseY - last_mouseY);
//     last_mouseX = mouseX;
//     last_mouseY = mouseY;
// }

// void tick(uint8_t*& data,size_t& size)
// {
//     // render->ClearAll();
//     // render->ClearScene(SBO);

//     for(size_t i = 0; i < width*height*4; ++i)
//     {
//         frame_buffer[i] = 0;
//     }

//     for(size_t i = 0; i < width*height; ++i)
//     {
//         zbuffer[i] = 100;
//     }
//     // FBO = render->CreateFrameBuffer(width, height, 4);
//     // ReiToEngine::Vec3d* triangle = new ReiToEngine::Vec3d[3];
//     ReiToEngine::Vec4d* colors = new ReiToEngine::Vec4d[3];
//     // uint32_t* line = new uint32_t[3];
//     // triangle[0] = ReiToEngine::Vec3d({0, 0, 0});
//     // triangle[1] = ReiToEngine::Vec3d({0, 100, 0});
//     // triangle[2] = ReiToEngine::Vec3d({100, 0, 0});

//     colors[0] = ReiToEngine::Vec4d({255, 0, 0, 255});
//     colors[1] = ReiToEngine::Vec4d({0, 255, 0, 255});
//     colors[2] = ReiToEngine::Vec4d({0, 0, 255, 255});
//     // line[0] = 0;
//     // line[1] = 1;
//     // line[2] = 2;

//     // size_t VBO = render->CreateVertexBuffer(triangle, nullptr, nullptr, colors, 3);
//     // size_t IBO = render->CreateIndiceBuffer(line, 3, false);
//     // size_t MBO = render->CreateMatrixRenderUnit();
//     // render->AppendMatrix(MBO, model);
//     // render->AppendMatrix(MBO, View);
//     // render->AppendMatrix(MBO, Projection);
//     // render->AppendMatrix(MBO, NDC);

//     // render->BindFrameBuffer(SBO, FBO);
//     // render->BindObject(SBO, VBO, IBO);
//     // render->BindMatrix(SBO, MBO);

//     { // draw the model
//         Matrix ModelView  = lookat(eye, center, ::Vec3f(0,1,0));
//         Matrix Projection = Matrix::identity(4);
//         Projection[3][2] = -1.f/(eye-center).norm();
//         Matrix ViewPort   = viewport(width/8, height/8, width*3/4, height*3/4);
//         std::cerr << ModelView << std::endl;
//         std::cerr << Projection << std::endl;
//         std::cerr << ViewPort << std::endl;
//         Matrix z = (ViewPort * Projection*ModelView);

//         ReiToEngine::Matrix4x4d model_v = ReiToEngine::Matrix4x4d({1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
//         ReiToEngine::Matrix4x4d View = camera->GetViewMatrix();
//         ReiToEngine::Matrix4x4d Projection_C = camera->GetProjectionMatrix();

//         ReiToEngine::Matrix4x4d target_C = ReiToEngine::Matrix4x4d({1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
//         ReiToEngine::Matrix4x4d NDC = camera->GetScreenMatrix(test1::width, test1::height, 0, 1, 0, 0);

//         target_C = Projection_C * View * model_v;

//         for (int i=0; i<model->nfaces(); i++) {
//             std::vector<int> face = model->face(i);
//             ReiToEngine::Vec4d screen_coords[3];
//             ReiToEngine::Vec4d world_coords[3];
//             float intensity[3];
//             for (int j=0; j<3; j++) {
//                 ::Vec3f v = model->vert(face[j]);

//                 ReiToEngine::Vec4d inner_v;

//                 inner_v.x = v.x;
//                 inner_v.y = v.y;
//                 inner_v.z = v.z;
//                 inner_v.w = 1;
//                 ReiToEngine::Vec4d temp = target_C * inner_v;
//                 temp.x = temp.x/temp.w;
//                 temp.y = temp.y/temp.w;
//                 temp.z = temp.z/temp.w;
//                 temp.w = 1;

//                 temp = NDC * temp;

//                 screen_coords[j].x = std::round(temp.x);
//                 screen_coords[j].y = std::round(temp.y);
//                 screen_coords[j].z = temp.z;
//                 screen_coords[j].w = temp.w;
//                 world_coords[j]  = inner_v;

//                 intensity[j] = model->norm(i, j)*light_dir;
//             }
//             triangle(screen_coords[0], screen_coords[1], screen_coords[2], intensity[0], intensity[1], intensity[2], *image, zbuffer);

//         }
//         image->write_tga_file("output.tga");
//     }

//     { // dump z-buffer (debugging purposes only)
//         TGAImage zbimage(width, height, TGAImage::GRAYSCALE);
//         for (int i=0; i<width; i++) {
//             for (int j=0; j<height; j++) {
//                 std::cout << "z:" << zbuffer[i+ j*width] << std::endl;
//                 std::cout << "z multy:" << zbuffer[i+ j*width] * 255.0 << std::endl;
//                 zbimage.set(i, j, TGAColor(zbuffer[i+j*width] * 255.0));
//             }
//         }
//         zbimage.write_tga_file("zbuffer.tga");
//     }
//     std::cout <<"ok";
//     int a;
//     std::cin >>a;
//     // render->DrawFrame(SBO, data, size);

//     // ReiToEngine::STBImageParser imageParser;
//     // ReiToEngine::Image image(width, height, 4);
//     // image.SetData(data);
//     // image.SetType(ReiToEngine::EImageType::IMAGE_PNG);

//     // imageParser.Write("./test1.png", image);
// }
// }
// #endif
