
#include"imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imguifilesystem.h"

#include <iostream>
#include"gl3w.h"
#include <GLFW/glfw3.h>

#include"MyGlWindow.h"
#include"Camera.h"

const char* glsl_version = "#version 130";

static int windowWidth = 1200;
static int windowHeight = 800;
static int screenWidth = 800;
static int screenHeight = 800;

bool firstMouse = true;
bool firstPosMouse = true;
bool isClicked = false;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float lastPosX = 800.0f / 2.0;
float lastPosY = 600.0 / 2.0;
float zoom = 45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

camera cam(glm::vec3(0.0f, -50.0f, 50.0f));

simulation::MyGlWindow* win;

const char* clickedObjName="";

static void ShowExampleAppMainMenuBar(bool* p_open);
static void ShowExampleMenuFile();
static void ShowGameObjectMenuFile();
static void ShowExampleAppLayout(bool* p_open);
static void ShowHierachyOverlay(bool* p_open);

static void ShowExampleAppFixedOverlay(bool* p_open)
{
	ImGui::SetNextWindowPos(ImVec2(windowWidth-200, 22));
	ImGui::SetNextWindowBgAlpha(0.5f);
	ImGui::SetNextWindowSize(ImVec2(200, windowHeight));
	if (!ImGui::Begin("Inspector", NULL,  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings| ImGuiWindowFlags_NoCollapse))
	{
		ImGui::End();
		return;
	}

	ImGui::Text(clickedObjName);

	if (isClicked) {
		//Object 정보 불러오기
		
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_Bullet)) {
			ImGui::Separator();
			ImGui::Text("x:%f\ny:%f\nz:%f", win->water->getPosition().x, win->water->getPosition().y, win->water->getPosition().z);
		}
	}

	ImGui::End();
}

/// <summary>
/// 하이어라키 화면
/// </summary>
/// <param name="p_open"></param>
static void ShowHierachyOverlay(bool* p_open)
{
	ImGui::SetNextWindowPos(ImVec2(0, 22));
	ImGui::SetNextWindowBgAlpha(0.5f);
	ImGui::SetNextWindowSize(ImVec2(200, windowHeight));

	if (!ImGui::Begin("Hierachy", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_NoCollapse))
	{
		ImGui::End();
		return;
	}
	
	if (win->rect != NULL) {
		if (ImGui::Button("rect")) {
			clickedObjName = "rect";
			isClicked = false;
		}
	}
	if (win->water != NULL) {	
		if (ImGui::Button(win->water->getName())) {
			clickedObjName = win->water->getName();
			isClicked = true;
		}
	}

	ImGui::End();
}

/// <summary>
/// 
/// </summary>
/// <param name="p_open"></param>
static void ShowExampleAppMainMenuBar(bool* p_open)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ShowExampleMenuFile();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("GameObject"))
		{
			ShowGameObjectMenuFile();
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

/// <summary>
/// 
/// </summary>
static void ShowExampleMenuFile()
{
	if (ImGui::MenuItem("Basic"))
	{

	}
	if (ImGui::MenuItem("Line"))
	{

	}
	if (ImGui::MenuItem("close"))
	{

	}
	if (ImGui::BeginMenu("Open Recent"))
	{
		ImGui::MenuItem("fish_hat.c");
		ImGui::MenuItem("fish_hat.inl");
		ImGui::MenuItem("fish_hat.h");
		if (ImGui::BeginMenu("More.."))
		{
			ImGui::MenuItem("Hello");
			ImGui::MenuItem("Sailor");
			if (ImGui::BeginMenu("Recurse.."))
			{
				ShowExampleMenuFile();
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
}

/// <summary>
/// 
/// </summary>
static void ShowGameObjectMenuFile()
{
	if (ImGui::MenuItem("Cube"))
	{

	}
	if (ImGui::MenuItem("Plane"))
	{

	}
	if (ImGui::MenuItem("Water"))
	{

	}
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	zoom -= (float)yoffset;
	if (zoom < 1.0f)
		zoom = 1.0f;
	if (zoom > 1000.0f)
		zoom = 1000.0f;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held

	ImGui::GetIO().MousePos.x = float(xposIn);
	ImGui::GetIO().MousePos.y = float(yposIn);

	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);


	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS) {
		if (firstMouse) 
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		cam.ProcessMouseMovement(xoffset, yoffset);
	}
	else {
		firstMouse = true;
	}

	//마우스 휠 버튼 클릭 감지
	int button = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
	if (button == GLFW_PRESS) {
		//캠 위치 이동
		if (firstPosMouse)
		{
			lastPosX = xpos;
			lastPosY = ypos;
			firstPosMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastPosX = xpos;
		lastPosY = ypos;

		cam.cameraPositionMove(xoffset, yoffset,deltaTime);
	}
	else {
		firstPosMouse = true;
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	win->setSize(width-400, height);
	win->setAspect((width- 400) / static_cast<float>(height));
}

int main()
{

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window;

	if (!glfwInit())
	{
		// Initialization failed
		glfwInit();
	}

	//무슨 버전을 쓸 것인가
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);			//오래된 opengl 을 쓰지 않겠다


	window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL FrameWork", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	if (gl3wInit()) {
		fprintf(stderr, "failed to initialize OpenGL\n");
		return -1;
	}

	if (!gl3wIsSupported(4, 3)) {
		fprintf(stderr, "OpenGL 4.0 not supported\n");
		return -1;
	}

	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

	win = new simulation::MyGlWindow(screenWidth, screenHeight);

	bool show_test_window = true;
	bool show_another_window = true;
	bool show_hierachy_window = true;
	bool show_menubar = false;
	bool show_app_main_menu_bar = true;

	ImVec4 clear_color = ImColor(144, 144, 144);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	io.Fonts->AddFontFromFileTTF("Cousine-Regular.ttf", 17.0f, NULL, io.Fonts->GetGlyphRangesKorean());

	// Setup ImGui binding
	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetWindowTitle(window, "MyGLwindow");
	glfwSetWindowSizeCallback(window, window_size_callback);

	//fltk와 다르게 한글이 지원된다. 
	glfwSetWindowTitle(window, u8"나의 opneGL윈도우");

	

	while (!glfwWindowShouldClose(window))
	{     //////////////////////////////

			// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Rendering
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		//glViewport(0, 0, display_w, display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		static bool show_app_main_menu_bar = true;

		if (show_app_main_menu_bar) ShowExampleAppMainMenuBar(&show_app_main_menu_bar);

		if (show_another_window)
		{
			ShowExampleAppFixedOverlay(&show_another_window);
		}
		if (show_hierachy_window) {
			ShowHierachyOverlay(&show_hierachy_window);
		}


		win->draw(cam, zoom);

		ImGui::Render();
		
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();

	}
	
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;

}
