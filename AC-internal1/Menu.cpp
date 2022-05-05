#include "Menu.h"
#include "config.h"

#include "utils/utils.h"

// forward declaration from imgui_imp_win32.h
IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static WNDPROC oldWndProc;

static LRESULT Wndproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (g->menu.open) {
        ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
        return true;
    }
    return CallWindowProc(oldWndProc, hWnd, uMsg, wParam, lParam);
}

bool Menu::initialize()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    window = utils::getWindow();

    ImGui_ImplWin32_Init(window);
    ImGui_ImplOpenGL2_Init();

    oldWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG)Wndproc);
    return true;
}

void Menu::render()
{
    if (!g->menu.open) return;

    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (g->menu.showDemo)
        ImGui::ShowDemoWindow(&g->menu.showDemo);

    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("AC-Internal"); 

        ImGui::Checkbox("Demo Window", &g->menu.showDemo);
        ImGui::Checkbox("Self", &showingSingleplayerWindow);
        ImGui::Checkbox("ESP", &showingESPWindow);
        ImGui::Checkbox("Aimbot", &showingAimbotWindow);
        ImGui::End();
    }

    if (g->menu.showAnother)
    {
        ImGui::Begin("Another Window", &g->menu.showAnother);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            g->menu.showAnother = false;
        ImGui::End();
    }

    if (showingSingleplayerWindow) showSingleplayerWindow();
    if (showingESPWindow) showESPWindow();
    if (showingAimbotWindow) showAimbotWindow();

    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void Menu::showSingleplayerWindow()
{
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_FirstUseEver);

    ImGui::Begin("Self", &showingSingleplayerWindow);

    ImGui::Text("Stats");

	if (ImGui::BeginTable("playerOptions", 2))
	{
		ImGui::TableNextColumn(); ImGui::SliderInt("Health", &g->player.setHealth, 0, 10000);
        ImGui::TableNextColumn(); ImGui::Checkbox("Set##health", &g->player.healthHack);
		ImGui::TableNextColumn(); ImGui::SliderInt("Armor", &g->player.setArmor, 0, 10000);
		ImGui::TableNextColumn(); ImGui::Checkbox("Set##armor", &g->player.armorHack);
		ImGui::TableNextColumn(); ImGui::SliderInt("Grenades", &g->player.setGranade, 0, 100);
		ImGui::TableNextColumn(); ImGui::Checkbox("Set##grenades", &g->player.granadeHack);
		ImGui::TableNextColumn(); ImGui::Checkbox("Unlimited ammo", &g->player.ammoHack);
		ImGui::TableNextColumn(); ImGui::Checkbox("Unlimited clip", &g->player.clipHack);
		ImGui::EndTable();
	}

    ImGui::Text("Movement");

    ImGui::Checkbox("No clip", &g->movement.noclipHack);

    ImGui::Text("Weapons");
    //if (ImGui::BeginListBox("##weaponlist", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
    //{
    //    for (int n = 0; n < IM_ARRAYSIZE(items); n++)
    //    {
    //        const bool is_selected = (item_current_idx == n);
    //        if (ImGui::Selectable(items[n], is_selected))
    //            item_current_idx = n;

    //        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
    //        if (is_selected)
    //            ImGui::SetItemDefaultFocus();
    //    }
    //    ImGui::EndListBox();
    //}

    ImGui::End();
}

void Menu::showESPWindow()
{
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_FirstUseEver);

    ImGui::Begin("ESP", &showingSingleplayerWindow);
    ImGui::Checkbox("Enable##ESP", &g->esp.enabled);
    ImGui::Checkbox("Distinguish between teams", &g->esp.distinguishTeams);
    ImGui::Checkbox("Show names", &g->esp.showNames);
    ImGui::Checkbox("Show health bar", &g->esp.showHealthBar);
    ImGui::End();
}

void Menu::showAimbotWindow()
{
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_FirstUseEver);

    ImGui::Begin("Aimbot", &showingAimbotWindow);
    ImGui::Checkbox("Enable##ESP", &g->aimbot.enabled);
    ImGui::Checkbox("Aim at team", &g->aimbot.aimAtTeam);
    ImGui::SliderFloat("Max lock distance", &g->aimbot.lockDistance, 5.0f, 1000.0f);

    ImGui::End();
}

Menu::~Menu()
{
    if (window != NULL)
        SetWindowLongPtr(window, GWLP_WNDPROC, (LONG)oldWndProc);
}
