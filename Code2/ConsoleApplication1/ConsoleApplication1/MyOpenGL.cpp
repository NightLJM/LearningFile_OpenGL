#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"shader.h"
#include "model.h"
#include"camera.h"
#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include<iostream>

using namespace std;
  //-----------------------------------定义摄像机--------------------------------------------
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));//???????(为什么输入三个参数就炸了)

float deltaTime=0.0f;
float lastTime=0.0f;
bool firstMouse = true;
float lastX = 540, lastY = 360;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);//帧刷新回调
void processInput(GLFWwindow *window);//按键监测
void mouse_callback(GLFWwindow* window, double xpos, double ypos);//鼠标移动监测
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);//鼠标滚动
unsigned int loadTexture(char const * path);//纹理加载函数
unsigned int loadCubemap(vector<std::string> faces);		//立方体纹理加载函数
int main()
{
	cout << "Content-type:text/html";
	cout << endl;
	cout << endl;
//-------------------------------窗口初始设置-------------------------------
	//设置版本
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//设置窗口
	int screenWidth = 1080;
	int screenHeight = 720;
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		system("PAUSE");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);//上下文为当前窗口

	//初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		system("PAUSE");
		return -1;
	}
	glViewport(0, 0, 1080, 720);	//视口
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//窗口大小调整的回调
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//让鼠标留在程序窗口
	glfwSetCursorPosCallback(window, mouse_callback);//注册鼠标回调函数
	glfwSetScrollCallback(window, scroll_callback);

//------------------------------------物体点数据---------------------------------
	/*float vertices[] = {		//点(v3),纹理(v2)
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f
	};*/
	float vertices[] = {
		// Back face
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f, // Bottom-left
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f, // top-right
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f, // bottom-right         
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f, // top-right
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f, // bottom-left
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f, // top-left
		 // Front face
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f, // top-right
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f, // top-right
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f, // top-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f, // bottom-left
		// Left face
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f, // top-right
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f, // top-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f, // bottom-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f, // top-right
		// Right face
	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f, // top-left
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f, // top-right         
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f, // top-left
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f, // bottom-left   
		 //Bottom face
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f, // top-right
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f, // top-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f, // top-right
		// Top face
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f, // top-left
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f, // bottom-right
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f, // top-right     
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f, // bottom-right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f, // top-left
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f  // bottom-left        
	};




	unsigned int indices[] = { // 索引，从0开始! 
		0, 1, 2, // 第一个三角形
		3, 4, 5,
		6, 7, 8,
		9, 10,11,
		12,13,14,
		15,16,17,
		18,19,20,
		21,22,23,
		24,25,26,
		27,28,29,
		30,31,32,
		33,34,35
	};

	float planevertices[] = {
		
		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};
	float transparentvertices[] = {

		0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  0.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  0.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  1.0f
	};

	float quadVertices[] = { 

		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	float skyboxVertices[] = {//天空盒
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	/*float points[] = {
		-0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // 左上
		0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // 右上
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f,// 右下
		-.5f, -0.5f, 1.0f, 1.0f, 0.0f // 左下
	};*/



//------------------------------------opengl帧缓冲------------------------------
	//---------------创建帧缓冲《帧缓冲1》---------------------
	unsigned int framebuffer;//创建并绑定帧缓冲
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	unsigned int texColorBuffer;//创建纹理
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	// 将它附加到当前绑定的帧缓冲对象
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
	unsigned int rbo;//创建渲染缓冲对象
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//将渲染缓冲对象附加到帧缓冲的深度和模板附件上
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	//查看是否绑定成功
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

//------------------------------------创建着色器程序---------------------------
	Shader shaderprogram = Shader("vertex.vs", "fragment.fs");//着色器源码不能用语言(如c++)格式
	Shader lightshader = Shader("lightvex.vs", "lightfrag.fs");//光源着色器
	Shader modelshader = Shader("model_loading.vs", "model_loading.gs", "model_loading.fs");//模型着色器
	Shader modelnormalshader = Shader("model_normal.vs", "model_normal.gs", "model_normal.fs");//模型法线可视化
	Shader stencilshader= Shader("stencil.vs", "stencil.fs");//模板测试着色器《模板测试1》
	Shader mixshader = Shader("mix.vs", "mix.fs");//混合测试着色器1
	Shader mixsshader = Shader("mixs.vs", "mixs.fs");//混合测试着色器1
	Shader Screenshader = Shader("framebuffers_screen.vs","framebuffers_screen.fs");//屏幕缓冲着色器
	Shader skyshader = Shader("skybox.vs","skybox.fs");//天空盒着色器
	//Shader vgfshader = Shader("geometery.vs", "geometery.gs", "geometery.fs");//几何着色器测试《几何着色器1》
	Shader planetshader = Shader("instancplanet.vs", "instancing.fs");//小行星模型的着色器（实例化）
	Shader rockshader = Shader("instancing.vs","instancing.fs");//陨石的着色器
//-----------------------------------绑定天空盒--------------------------------------
	//绑定VAO
	unsigned int skyVAO;
	glGenVertexArrays(1, &skyVAO);
	glBindVertexArray(skyVAO);//在调用着色器前要绑定一次
	//创建顶点缓冲对象VBO，并将数据复制到VBO
	unsigned int skyVBO;
	glGenBuffers(1, &skyVBO);//生成缓冲对象
	glBindBuffer(GL_ARRAY_BUFFER, skyVBO);//将VBO绑定到缓冲对象
	//glBufferData(GL_ARRAY_BUFFER, sizeof(verticestest), verticestest, GL_STATIC_DRAW);//将顶点复制到VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);//将顶点复制到VBO
	//链接物体顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
//-----------------------------------绑定物体的点-------------------------------------
	//绑定VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);//在调用着色器前要绑定一次
	//创建顶点缓冲对象VBO，并将数据复制到VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);//生成缓冲对象
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//将VBO绑定到缓冲对象
	//glBufferData(GL_ARRAY_BUFFER, sizeof(verticestest), verticestest, GL_STATIC_DRAW);//将顶点复制到VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//将顶点复制到VBO
	//绑定EBO
	unsigned int EBO;
	glGenBuffers(1, &EBO);//创建对象
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//复制索引
//------------------------------------链接物体----------------------------------------------
	//链接物体顶点属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	//链接纹理属性
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(4);//!!!!!!!!!!!让着色器计算法向量矩阵会增加其开销，可先再程序中计算然后通过uniform传递

//------------------------------------绑定光源的点---------------------------------
	//绑定VAO
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);//在调用着色器前要绑定一次
	//创建顶点缓冲对象VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//将VBO绑定到缓冲对象
	//绑定EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//------------------------------------链接光源-----------------------------
	//链接光源顶点属性
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(3);

//-----------------------------------绑定链接接地板和草--------------------
	unsigned int planeVAO;//地板
	glGenVertexArrays(1, &planeVAO);
	glBindVertexArray(planeVAO);//在调用着色器前要绑定一次
	//创建顶点缓冲对象VBO
	unsigned int planeVBO;
	glGenBuffers(1, &planeVBO);//生成缓冲对象
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);//将VBO绑定到缓冲对象
	//glBufferData(GL_ARRAY_BUFFER, sizeof(verticestest), verticestest, GL_STATIC_DRAW);//将顶点复制到VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(planevertices), planevertices, GL_STATIC_DRAW);//将顶点复制到VBO
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(6);

	unsigned int transparentVAO;//草
	glGenVertexArrays(1, &transparentVAO);
	glBindVertexArray(transparentVAO);//在调用着色器前要绑定一次
	//创建顶点缓冲对象VBO
	unsigned int transparentVBO;
	glGenBuffers(1, &transparentVBO);//生成缓冲对象
	glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);//将VBO绑定到缓冲对象
	//glBufferData(GL_ARRAY_BUFFER, sizeof(verticestest), verticestest, GL_STATIC_DRAW);//将顶点复制到VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentvertices), transparentvertices, GL_STATIC_DRAW);//将顶点复制到VBO
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(6);

	unsigned int cVAO;//物体
	glGenVertexArrays(1, &cVAO);
	glBindVertexArray(cVAO);//在调用着色器前要绑定一次
	//创建顶点缓冲对象VBO
	unsigned int cVBO;
	glGenBuffers(1, &cVBO);//生成缓冲对象
	glBindBuffer(GL_ARRAY_BUFFER, cVBO);//将VBO绑定到缓冲对象
	//glBufferData(GL_ARRAY_BUFFER, sizeof(verticestest), verticestest, GL_STATIC_DRAW);//将顶点复制到VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//将顶点复制到VBO
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(6);

//-----------------------------------屏幕长方体绑定----------------------------
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	
	//几何着色器测试点绑定《几何着色器2》
	/*
	unsigned int pVAO;
	glGenVertexArrays(1, &pVAO);
	glBindVertexArray(pVAO);
	unsigned int pVBO;
	glGenBuffers(1, &pVBO);
	glBindBuffer(GL_ARRAY_BUFFER, pVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	*/
//-----------------------------------绑定纹理图片------------------------------
	//天空盒纹理
	vector<std::string> faces
	{
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces);

	//纹理
	stbi_set_flip_vertically_on_load(true);//调整坐标轴
	unsigned int texture1;
	texture1 = loadTexture("9.jpg");
	unsigned int texture2;
	texture2 = loadTexture("9-1.jpg");
	//glActiveTexture(GL_TEXTURE14);
	//glBindTexture(GL_TEXTURE_2D, texture1);//激活纹理
	//glActiveTexture(GL_TEXTURE15);
	//glBindTexture(GL_TEXTURE_2D, texture2);//激活纹理
	unsigned int texture3;
	texture3 = loadTexture("marble.jpg");
	unsigned int texture4;
	texture4 = loadTexture("metal.png");
	unsigned int texture5;
	texture5 = loadTexture("grass.png");
	unsigned int texture6;
	texture6 = loadTexture("window.png");
	unsigned int texture7;
	texture7 = loadTexture("container.jpg");

	vector<glm::vec3> vegetation  //草的相对位置
	{
		glm::vec3(0.5f, 0.001f, 1.51f),
		glm::vec3(-2.0f, 0.0f, -2.0f),
		glm::vec3(0.0f, 0.0f, 0.7f),
		glm::vec3(-0.3f, 0.0f, -2.3f),
		glm::vec3(0.5f, 0.0f, -0.6f)
	};

	vector<glm::vec3> windows  //草的相对位置
	{
		glm::vec3(-3.5f, 0.001f, 1.71f),
		glm::vec3(-5.0f, 0.0f, -3.0f),
		glm::vec3(-3.0f, 0.0f, 2.7f),
		glm::vec3(-3.3f, 0.0f, -1.3f),
		glm::vec3(-3.5f, 0.0f, -1.6f)
	};

//------------------------------------模型加载-----------------------------------------
	Model ourModel("nanosuit/nanosuit.obj");//人物模型
	Model planetModel("planet/planet.obj");//小行星（实例化）
	Model rockModel("rock/rock.obj");//陨石（实例化）

//-----------------------------小行星与陨石的矩阵计算与存储---------------------
	unsigned int amount = 10000;
	glm::mat4 *modelMatrices;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); // 初始化随机种子    
	float radius = 50.0;
	float offset = 2.5f;
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model;
		model= glm::translate(model, glm::vec3(-70.0f,40.0f,0.0f));
		// 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. 缩放：在 0.05 和 0.25f 之间缩放
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. 添加到矩阵的数组中
		modelMatrices[i] = model;
	}
	//将上述矩阵数据存入缓冲（实例化）
	unsigned int instancebuffer;
	glGenBuffers(1, &instancebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, instancebuffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
	for (unsigned int i = 0; i < rockModel.meshes.size(); i++)
	{
		unsigned int rockiVAO = rockModel.meshes[i].VAO;
		glBindVertexArray(rockiVAO);
		// 顶点属性
		GLsizei vec4Size = sizeof(glm::vec4);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));
		//glVertexAttribDivisor绑定的实例化数组跟在那个VAO后面，就是那个VAO的实例化数组
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}


//------------------------------------每一帧处理部分-----------------------------------
	/*
	glEnable(GL_DEPTH_TEST);//开启深度缓冲
	//glDepthFunc(GL_ALWAYS);//总是通过深度缓冲
	//模板缓冲测试《模板测试2》
	glEnable(GL_STENCIL_TEST);
	*/

	float timeValue;
	float greenValue;
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//帧缓冲《2》
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST);//开启深度缓冲
		glDepthFunc(GL_LEQUAL);//小于等于深度值时通过缓冲（因为天空盒的深度值为1,为了让天空盒通过测试）
		//模板缓冲测试《模板测试2》
		glEnable(GL_STENCIL_TEST);    //如果没有创建的帧缓冲，可以将两个测试enable放在循环外

		glClearColor(0, 0, 0, 1.0f);
		//《模板测试3》
		glStencilMask(0xFF);//为了模板缓冲能清除
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);//清空颜色缓冲和深度缓冲(GL_STENCIL_BUFFER_BIT 是模板缓冲)
		
		//《模板测试4》
		glStencilMask(0x00);//模板缓冲掩码，为了在绘制物体前模板缓冲不变

//--------------------------------每一帧时间------------------------------------------
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

//--------------------------------画光源-------------------------------------------------
		glEnable(GL_CULL_FACE);//开启面剔除《1》
		glCullFace(GL_BACK);//只剔除背向面
		glFrontFace(GL_CCW);//逆时针为正向面

		glm::vec3 light(1.0f, 1.0f, 1.0f);
		lightshader.use();
		glBindVertexArray(lightVAO);
		//光源变换矩阵
		glm::vec3 lightPos(1.2f, 1.0f, 2.0f);//光源位置
		glm::mat4 modelLight;
		modelLight = glm::translate(modelLight, lightPos);
		modelLight = glm::scale(modelLight, glm::vec3(0.2f));
		glm::mat4 viewLight;
		viewLight = camera.GetViewMatrix();
		glm::mat4 projectionLight;
		projectionLight = glm::perspective(glm::radians(camera.Zoom), (float)(screenWidth / screenHeight), 0.1f, 100.0f);
		//更新uniform变换矩阵
		lightshader.setMat("model", modelLight);
		lightshader.setMat("view", viewLight);
		lightshader.setMat("projection", projectionLight);
		// 更新uniform颜色;
		lightshader.setFloat4("lightcolor", light.x, light.y, light.z, 1.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

//--------------------------------画物体-------------------------------------------------
		//《模板测试5》
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);//通过测试就将模板缓冲的值设为标准值
		glStencilFunc(GL_ALWAYS, 1, 0xFF);//永远通过测试
		glStencilMask(0xFF);//模板缓冲掩码

		glActiveTexture(GL_TEXTURE14);
		glBindTexture(GL_TEXTURE_2D, texture1);//激活纹理
		glActiveTexture(GL_TEXTURE15);
		glBindTexture(GL_TEXTURE_2D, texture2);//激活纹理
		shaderprogram.use();//激活shaderProgram
		glBindVertexArray(VAO);
		//摄像机相关||变换矩阵
		glm::mat4 model;//局部坐标--->世界坐标
		glm::mat4 view;//世界坐标--->观察坐标
		view = camera.GetViewMatrix();
		glm::mat4 projection;//观察坐标--->裁剪坐标
		projection = glm::perspective(glm::radians(camera.Zoom), (float)(screenWidth / screenHeight), 0.1f, 100.0f);
		//更新uniform变换矩阵
		shaderprogram.setMat("model", model);
		shaderprogram.setMat("view", view);
		shaderprogram.setMat("projection", projection);
		//更新uniforms摄像机位置
		shaderprogram.setFloat4("viewPosition", camera.Position.x, camera.Position.y, camera.Position.z, 1.0f);
		//更新uniform材质
		shaderprogram.setInt("material.diffuse", 14);
		shaderprogram.setInt("material.specular", 15);
		shaderprogram.setFloat1("material.shininess", 32.0f);
		//更新uniform点光源(平行光源)
		shaderprogram.setFloat4("light.posordir", lightPos.x, lightPos.y, lightPos.z, 1.0f);//光源
		shaderprogram.setFloat4("light.lightcolor", light.x, light.y, light.z, 1.0f);
		shaderprogram.setFloat4("light.ambient", 0.2f, 0.2f, 0.2f, 1.0f);
		shaderprogram.setFloat4("light.diffuse", 0.5f, 0.5f, 0.5f, 1.0f); // 将光照调暗了一些以搭配场景
		shaderprogram.setFloat4("light.specular", 1.0f, 1.0f, 1.0f, 1.0f);
		shaderprogram.setFloat1("light.constant", 1.0f);
		shaderprogram.setFloat1("light.linear", 0.09f);
		shaderprogram.setFloat1("light.quadratic", 0.032f);
		//更新uniform聚光
		shaderprogram.setFloat4("camera.position", camera.Position.x, camera.Position.y, camera.Position.z, 1.0f);
		shaderprogram.setFloat4("camera.front", camera.Front.x, camera.Front.y, camera.Front.z, 1.0f);
		shaderprogram.setFloat4("camera.lightcolor", 1.0f, 1.0f, 1.0f, 1.0f);
		shaderprogram.setFloat4("camera.diffuse", 0.5f, 0.5f, 0.5f, 1.0f); // 将光照调暗了一些以搭配场景
		shaderprogram.setFloat4("camera.specular", 1.0f, 1.0f, 1.0f, 1.0f);
		shaderprogram.setFloat1("camera.constant", 1.0f);
		shaderprogram.setFloat1("camera.linear", 0.04f);
		shaderprogram.setFloat1("camera.quadratic", 0.010f);
		shaderprogram.setFloat1("camera.cutoff", glm::cos(glm::radians(3.0f)));
		shaderprogram.setFloat1("camera.outcutoff", glm::cos(glm::radians(9.0f)));
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glDisable(GL_CULL_FACE);//关闭面剔除《2》

		//画轮廓《模板测试6》
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);//之前绘制了物体的地方现在过不了模板缓冲
		glStencilMask(0x00);//保持模板缓冲不变
		//glDisable(GL_DEPTH_TEST);//关闭深度测试，放大后的物体可能被覆盖
		stencilshader.use();
		glBindVertexArray(VAO);
		glm::mat4 stencilmodel;//局部坐标--->世界坐标
		stencilmodel = glm::scale(stencilmodel, glm::vec3(1.2f));//放大物体
		glm::mat4 stencilview;//世界坐标--->观察坐标
		stencilview = camera.GetViewMatrix();
		glm::mat4 stencilprojection;//观察坐标--->裁剪坐标
		stencilprojection = glm::perspective(glm::radians(camera.Zoom), (float)(screenWidth / screenHeight), 0.1f, 100.0f);
		//更新uniform变换矩阵
		stencilshader.setMat("model", stencilmodel);
		stencilshader.setMat("view", stencilview);
		stencilshader.setMat("projection", stencilprojection);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//glEnable(GL_DEPTH_TEST);//开启深度测试
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glDisable(GL_STENCIL_TEST);

//----------------------------------画模型---------------------------------------
		glEnable(GL_CULL_FACE);//开启面剔除《3》
		glCullFace(GL_BACK);//只剔除背向面
		glFrontFace(GL_CCW);//逆时针为正向面

		modelshader.use();
		//更新uniform变换矩阵
		glm::mat4 modelmodel;
		modelmodel = glm::translate(modelmodel, glm::vec3(2.0f, -1.75f, 0.0f));
		modelmodel = glm::scale(modelmodel, glm::vec3(0.2f, 0.2f, 0.2f));
		glm::mat4 modelview;
		modelview = camera.GetViewMatrix();
		glm::mat4 modelprojection;
		modelprojection = glm::perspective(glm::radians(camera.Zoom), (float)(screenWidth / screenHeight), 0.1f, 100.0f);
		modelshader.setMat("model", modelmodel);
		modelshader.setMat("view", modelview);
		modelshader.setMat("projection", modelprojection);
		//更新uniforms摄像机位置
		modelshader.setFloat4("viewPosition", camera.Position.x, camera.Position.y, camera.Position.z, 1.0f);
		//更新uniform点光源(平行光源)
		modelshader.setFloat4("light.posordir", lightPos.x, lightPos.y, lightPos.z, 1.0f);//光源
		modelshader.setFloat4("light.lightcolor", light.x, light.y, light.z, 1.0f);
		modelshader.setFloat4("light.ambient", 0.2f, 0.2f, 0.2f, 1.0f);
		modelshader.setFloat4("light.diffuse", 0.5f, 0.5f, 0.5f, 1.0f); // 将光照调暗了一些以搭配场景
		modelshader.setFloat4("light.specular", 1.0f, 1.0f, 1.0f, 1.0f);
		modelshader.setFloat1("light.constant", 1.0f);
		modelshader.setFloat1("light.linear", 0.09f);
		modelshader.setFloat1("light.quadratic", 0.032f);
		//更新uniform聚光
		modelshader.setFloat4("camera.position", camera.Position.x, camera.Position.y, camera.Position.z, 1.0f);
		modelshader.setFloat4("camera.front", camera.Front.x, camera.Front.y, camera.Front.z, 1.0f);
		modelshader.setFloat4("camera.lightcolor", 1.0f, 1.0f, 1.0f, 1.0f);
		modelshader.setFloat4("camera.diffuse", 0.5f, 0.5f, 0.5f, 1.0f); // 将光照调暗了一些以搭配场景
		modelshader.setFloat4("camera.specular", 1.0f, 1.0f, 1.0f, 1.0f);
		modelshader.setFloat1("camera.constant", 1.0f);
		modelshader.setFloat1("camera.linear", 0.04f);
		modelshader.setFloat1("camera.quadratic", 0.010f);
		modelshader.setFloat1("camera.cutoff", glm::cos(glm::radians(3.0f)));
		modelshader.setFloat1("camera.outcutoff", glm::cos(glm::radians(9.0f)));
		ourModel.Draw(modelshader);
		glDisable(GL_CULL_FACE);//关闭面剔除《4》

		//法线可视化
		modelnormalshader.use();
		modelnormalshader.setMat("model", modelmodel);
		modelnormalshader.setMat("view", modelview);
		modelnormalshader.setMat("projection", modelprojection);
		ourModel.Draw(modelnormalshader);

//----------------------------------小行星模型-------------------------------------------------
		/*
		//不用实例化,卡的一批
		//画小行星
		planetshader.use();
		planetshader.setMat("view", modelview);
		planetshader.setMat("projection", modelprojection);
		glm::mat4 plamodel;
		plamodel = glm::translate(plamodel, glm::vec3(-70.0f, 40.0f, 0.0f));
		plamodel = glm::scale(plamodel, glm::vec3(4.0f, 4.0f, 4.0f));
		planetshader.setMat("model", plamodel);
		planetModel.Draw(planetshader);
		//画陨石
		for (unsigned int i = 0; i < amount; i++)
		{
			planetshader.setMat("model", modelMatrices[i]);
			rockModel.Draw(planetshader);
		}
		*/
		//用实例化
		//画小行星
		planetshader.use();
		planetshader.setMat("view", modelview);
		planetshader.setMat("projection", modelprojection);
		glm::mat4 plamodel;
		plamodel = glm::translate(plamodel, glm::vec3(-70.0f, 40.0f, 0.0f));
		plamodel = glm::scale(plamodel, glm::vec3(4.0f, 4.0f, 4.0f));
		planetshader.setMat("model", plamodel);
		planetModel.Draw(planetshader);

		// 绘制小行星
		rockshader.use();
		rockshader.setMat("view", modelview);
		rockshader.setMat("projection", modelprojection);
		for (unsigned int i = 0; i < rockModel.meshes.size(); i++)
		{
			glBindVertexArray(rockModel.meshes[i].VAO);
			glDrawElementsInstanced(
				GL_TRIANGLES, rockModel.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount
			);
		}
//----------------------------------画地板和草《混合测试》--------------------------------------
		glBindVertexArray(planeVAO);//地板
		glActiveTexture(GL_TEXTURE14);
		glBindTexture(GL_TEXTURE_2D, texture3);//激活纹理
		mixshader.use();
		glm::vec3 relativePos(-3.0f, 0.0f, 0.0f);
		glm::mat4 planemodel;
		planemodel = glm::translate(planemodel, relativePos);
		mixshader.setMat("model", planemodel);
		mixshader.setMat("view", view);
		mixshader.setMat("projection", projection);
		mixshader.setInt("texture", 14);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		glEnable(GL_CULL_FACE);//开启面剔除《5》
		glCullFace(GL_BACK);//只剔除背向面
		glFrontFace(GL_CCW);//逆时针为正向面

		glBindVertexArray(cVAO);//箱子
		glActiveTexture(GL_TEXTURE14);
		glBindTexture(GL_TEXTURE_2D, texture4);//激活纹理
		mixshader.use();
		glm::mat4 cmodel;
		cmodel = glm::translate(cmodel, relativePos);
		cmodel = glm::translate(cmodel, glm::vec3(1.0f, 0.001f, 1.0f));
		mixshader.setMat("model", cmodel);
		mixshader.setMat("view", view);
		mixshader.setMat("projection", projection);
		mixshader.setInt("texture", 14);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		cmodel = glm::translate(cmodel, glm::vec3(-2.0f, -0.0f, -2.0f));
		mixshader.setMat("model", cmodel);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//----在画两个箱子--------
		glActiveTexture(GL_TEXTURE14);
		glBindTexture(GL_TEXTURE_2D, texture7);
		mixshader.setInt("texture", 14);
		cmodel = glm::translate(cmodel, glm::vec3(-2.0f, 0.0f, 0.0f));
		mixshader.setMat("model", cmodel);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		cmodel = glm::translate(cmodel, glm::vec3(0.0f, 0.0f, -2.0f));
		mixshader.setMat("model", cmodel);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDisable(GL_CULL_FACE);//关闭面剔除《6》

		//---------------草------------------
		glBindVertexArray(transparentVAO);//草
		glActiveTexture(GL_TEXTURE14);
		glBindTexture(GL_TEXTURE_2D, texture5);//激活纹理
		mixshader.use();
		glm::mat4 transparentmodel;
		transparentmodel = glm::translate(transparentmodel, relativePos);
		//mixshader.setMat("model", planemodel);
		mixshader.setMat("view", view);
		mixshader.setMat("projection", projection);
		mixshader.setInt("texture", 14);
		for (GLuint i = 0; i < vegetation.size(); i++)
		{
			transparentmodel = glm::translate(transparentmodel, vegetation[i]);
			mixshader.setMat("model", transparentmodel);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		//-------------------------------画几何着色器的点《几何着色器3》-------------------------------------
		//vgfshader.use();
		//glBindVertexArray(pVAO);
		//glDrawArrays(GL_POINTS, 0, 4);

		//--------------------------------画天空盒---------------------------------------------
		//glDepthMask(GL_FALSE);
		skyshader.use();
		glm::mat4 skyprojection;
		skyprojection = glm::perspective(glm::radians(camera.Zoom), (float)(screenWidth / screenHeight), 0.1f, 100.0f);
		glm::mat4 skyview;
		//skyview = camera.GetViewMatrix();
		skyview = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glBindVertexArray(skyVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		skyshader.setMat("projection", skyprojection);
		skyshader.setMat("view", skyview);
		skyshader.setInt("skybox", 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDepthMask(GL_TRUE);

		//-------------窗户------------------
		glEnable(GL_BLEND);//开启混合
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindVertexArray(transparentVAO);//玻璃
		glActiveTexture(GL_TEXTURE14);
		glBindTexture(GL_TEXTURE_2D, texture6);//激活纹理
		mixsshader.use();
		mixsshader.setMat("view", view);
		mixsshader.setMat("projection", projection);
		mixsshader.setInt("texture", 14);
		glm::mat4 winmodelor;
		glm::mat4 winmodel;
		std::map<float, glm::vec3> sorted;

		/*
		for (GLuint i = 0; i <  windows.size(); i++)//未排序
		{
			winmodel = glm::translate(winmodelor, windows[i]);
			mixsshader.setMat("model", winmodel);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		*/
		
		for (unsigned int i = 0; i < windows.size(); i++)//经过排序
		{
			float distance = glm::length(camera.Position - windows[i]);
			sorted[distance] = windows[i];
		}
		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			winmodel = glm::translate(winmodelor, it->second);
			mixsshader.setMat("model", winmodel);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		glDisable(GL_BLEND);//关闭混合

		//-----《帧缓冲3》-----
		glDisable(GL_DEPTH_TEST);//关闭两个测试，由于后面只画一长方形片，因此开着浪费资源
		glDisable(GL_STENCIL_TEST);//关闭模板测试《模板测试7》
		glBindFramebuffer(GL_FRAMEBUFFER, 0);//还原系统帧缓冲
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE15);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		Screenshader.use();
		Screenshader.setInt("screenTexture", 15);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//for (unsigned int i = 0; i < 10; i++)
		//{
		//	glm::mat4 model1;
		//	model = glm::translate(model1, glm::vec3((float)i,0.0f,0.0f));
		//	shaderprogram.setMat("model", model);
		//
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &lightVAO);;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)		
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	//std::cout <<"xpose:"<< xpos << endl;
	float yoffset = lastY - ypos;
	//std::cout << "ypose:" << ypos << endl;
	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMovement(xoffset, yoffset,true);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const * path)		//二维纹理加载函数
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		//为当前绑定的纹理对象设置环绕、过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}


unsigned int loadCubemap(vector<std::string> faces)		//立方体纹理加载函数
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}