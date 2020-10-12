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
float lastX = 400, lastY = 300;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);//帧刷新回调
void processInput(GLFWwindow *window);//按键监测
void mouse_callback(GLFWwindow* window, double xpos, double ypos);//鼠标移动监测
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);//鼠标滚动

int main()
{
//-------------------------------窗口初始设置-------------------------------
	//设置版本
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//设置窗口
	int screenWidth = 800;
	int screenHeight = 600;
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
	glViewport(0, 0, 800, 600);	//视口
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//窗口大小调整的回调
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//让鼠标留在程序窗口
	glfwSetCursorPosCallback(window, mouse_callback);//注册鼠标回调函数
	glfwSetScrollCallback(window, scroll_callback);

//------------------------------------物体点数据---------------------------------
	float vertices[] = {		//点(v3),纹理(v2)
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
//------------------------------------创建着色器程序---------------------------
	Shader shaderprogram = Shader("vertex.vs", "fragment.fs");//着色器源码不能用语言(如c++)格式
	Shader lightshader = Shader("lightvex.vs", "lightfrag.fs");//光源着色器
	Shader modelshader = Shader("model_loading.vs","model_loading.fs");//模型着色器

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


//-----------------------------------绑定纹理图片(待优化）------------------------------
	//纹理
	stbi_set_flip_vertically_on_load(true);//调整坐标轴
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	//为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//加载并生成纹理
	int width, height, nrChannels;
	unsigned char *data = stbi_load("9.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	//为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//加载并生成纹理
	//int width, height, nrChannels;
	data = stbi_load("9-1.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	glActiveTexture(GL_TEXTURE14);
	glBindTexture(GL_TEXTURE_2D, texture1);//激活纹理
	glActiveTexture(GL_TEXTURE15);
	glBindTexture(GL_TEXTURE_2D, texture2);//激活纹理

//------------------------------------模型加载-----------------------------------------
	Model ourModel("nanosuit/nanosuit.obj");

//------------------------------------每一帧处理部分-----------------------------------
	glEnable(GL_DEPTH_TEST);//开启深度缓冲
	//glDepthFunc(GL_ALWAYS);//总是通过深度缓冲
	float timeValue;
	float greenValue;
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清空颜色缓冲和深度缓冲
//--------------------------------每一帧时间------------------------------------------
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

//--------------------------------画光源-------------------------------------------------
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

//----------------------------------画模型---------------------------------------
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