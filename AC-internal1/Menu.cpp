#include "Menu.h"
#include "config.h"

// forward declaration from imgui_imp_win32.h
IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static WNDPROC oldWndProc;

static LRESULT Wndproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (config::menuOpen) {
        ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
        return true;
    }
    return CallWindowProc(oldWndProc, hWnd, uMsg, wParam, lParam);
}

bool Menu::initialize(LPCWSTR gameWindowName)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    window = FindWindow(NULL, gameWindowName);
    if (window == NULL) {
        return false;
    }

    ImGui_ImplWin32_Init(window);
    ImGui_ImplOpenGL2_Init();

    oldWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG)Wndproc);
    return true;
}

void Menu::render()
{
    if (!config::menuOpen) return;

    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (config::show_demo_window)
        ImGui::ShowDemoWindow(&config::show_demo_window);

    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &config::show_demo_window);      // Edit bools storing our window open/close state
        if(!config::isMultiplayer) ImGui::Checkbox("Singleplayer", &showingSingleplayerWindow);
        ImGui::Checkbox("ESP", &showingESPWindow);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    if (config::show_another_window)
    {
        ImGui::Begin("Another Window", &config::show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            config::show_another_window = false;
        ImGui::End();
    }

    if (showingSingleplayerWindow) showSingleplayerWindow();
    if (showingESPWindow) showESPWindow();

    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void Menu::showSingleplayerWindow()
{
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_FirstUseEver);

    ImGui::Begin("Singleplayer", &showingSingleplayerWindow);

	if (ImGui::BeginTable("playerOptions", 2))
	{
		ImGui::TableNextColumn(); ImGui::SliderInt("Health", &config::healthToSet, 0, 10000);
		ImGui::TableNextColumn(); ImGui::Checkbox("Set##health", &config::healthHack);
		ImGui::TableNextColumn(); ImGui::SliderInt("Armor", &config::armorToSet, 0, 10000);
		ImGui::TableNextColumn(); ImGui::Checkbox("Set##armor", &config::armorHack);
		ImGui::TableNextColumn(); ImGui::SliderInt("Grenades", &config::granadeToSet, 0, 100);
		ImGui::TableNextColumn(); ImGui::Checkbox("Set##grenades", &config::granadeHack);
		ImGui::TableNextColumn(); ImGui::SliderInt("Weapon ammo", &config::carabineAmmoToSet, 0, 50);
		ImGui::TableNextColumn(); ImGui::Checkbox("Set##weaponammo", &config::carabineAmmoHack);
        ImGui::TableNextColumn(); ImGui::Checkbox("No clip", &config::noclipHack);
		ImGui::EndTable();
	}

    ImGui::End();
}

void Menu::showESPWindow()
{
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_FirstUseEver);

    ImGui::Begin("ESP", &showingSingleplayerWindow);
    ImGui::Checkbox("Enable##ESP", &config::espHack);
    ImGui::Checkbox("Distinguish between teams", &config::esp_distinguishTeams);
    ImGui::Checkbox("Show names", &config::esp_showNames);
    ImGui::Checkbox("Show health bar", &config::esp_showHealthBar);
    ImGui::End();
}

Menu::~Menu()
{
    if (window != NULL)
        SetWindowLongPtr(window, GWLP_WNDPROC, (LONG)oldWndProc);
}
