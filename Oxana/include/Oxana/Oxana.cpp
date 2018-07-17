#include "Oxana.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

namespace Oxana
{
	GUI::GUI(const Settings & settings) :settings(settings)
	{
	}

	GUI::Settings::Settings() :
		title("Graphical Driver"),
		framerate(60),
		width(1024),
		height(768),
		background(sf::Color(152, 152, 152, 255)),
		showSTDOUT(false),
		stdoutCaptureFile("GDOutput.txt"),
		showOverlay(true)
	{
	}

	void GUI::Initialize()
	{
		this->Assert();
		this->window = std::make_unique<sf::RenderWindow>(sf::VideoMode(this->settings.width, this->settings.height), this->settings.title);
		this->window->setKeyRepeatEnabled(false);
		ImGui::SFML::Init(*(this->window));
		//outputFile = std::freopen(settings.stdoutCaptureFile.c_str(), "w+", stdout);
		//this->Set(&this->simulations[0]);
	}

	void GUI::Shutdown()
	{
		// Once the window has closed, shutdown
		std::fclose(stdout);
		ImGui::SFML::Shutdown();
	}

	void GUI::Assert()
	{
		this->hasSimulations = !this->simulations.empty();
		for (auto& simulation : this->simulations)
		{

		}

		this->hasTests = !this->tests.empty();
		for (auto& test : this->tests)
		{

		}
	}

	void GUI::Set(Simulation * simulation)
	{
		this->currentSimulation = simulation;
		this->showResult = false;
		this->currentSimulation->onDone = [&]() { this->showResult = true; };
		this->currentSimulation->Initialize();
	}

	void GUI::Run(Test * test)
	{
		test->Run();
		test->enabled = true;
	}
	
	void GUI::Run()
	{
		Initialize();

		sf::Clock deltaClock;

		while (this->window->isOpen())
		{
			sf::Event event;
			while (this->window->pollEvent(event))
			{
				ImGui::SFML::ProcessEvent(event);
				if (event.type == sf::Event::Closed)
					this->window->close();
				if (event.type == sf::Event::KeyPressed)
					OnInput(event.key.code);
			}

			ImGui::SFML::Update(*window, deltaClock.restart());
			Update();

			window->clear(settings.background);
			ImGui::SFML::Render(*window);
			window->display();
		}

		Shutdown();
	}


	void GUI::Add(Simulation simulation)
	{
		simulations.push_back(simulation);
	}

	void GUI::Add(Test test)
	{
		tests.push_back(test);
	}

	void GUI::Update()
	{
		if (this->currentSimulation && this->currentSimulation->isAutomatic)
			this->currentSimulation->OnStep();

		this->Draw();
	}

	void GUI::Draw()
	{
		DrawMenu();
		DrawTests();

		if (currentSimulation != nullptr)
			DrawSimulation();
	}

	void GUI::OnInput(sf::Keyboard::Key key)
	{
		switch (key)
		{
			case sf::Keyboard::Left:
				this->currentSimulation->Step(StepType::Backward);
				break;
			case sf::Keyboard::Right:
				this->currentSimulation->Step(StepType::Forward);
				break;
			case sf::Keyboard::Up:
				this->currentSimulation->Step(StepType::Once);
				break;
			case sf::Keyboard::Space:
				this->currentSimulation->Step(StepType::Multiple);
				break;
			case sf::Keyboard::F8:
				this->currentSimulation->Reset();
				break;
		}
	}

	void GUI::DrawMenu()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (this->hasSimulations)
			{
				if (ImGui::BeginMenu("Simulations"))
				{
					for (auto& simulation : this->simulations)
					{
						if (ImGui::MenuItem(simulation.name.c_str())) {
							this->Set(&simulation);
						}
					}

					ImGui::EndMenu();
				}
			}
			
			if (this->hasTests)
			{
				if (ImGui::BeginMenu("Tests"))
				{
					for (auto& test : this->tests)
					{
						if (ImGui::MenuItem(test.name.c_str())) {
							this->Run(&test);
						}
					}

					ImGui::EndMenu();
				}
			}

			ImGui::EndMainMenuBar();
		}
	}

	void GUI::DrawControls()
	{
		const int min = this->currentSimulation->Min();
		const int max = this->currentSimulation->Max();

		ImGui::TextColored(sf::Color::Red, "Steps");
		ImGui::Text(std::string("- Total: " + std::to_string(this->currentSimulation->steps)).c_str());
		ImGui::Text(std::string("- Buffer size: " + std::to_string(this->currentSimulation->stepBufferSize)).c_str());
		ImGui::Text(std::string("- Step Multiplier: " + std::to_string(this->currentSimulation->stepMultiplier)).c_str());

		ImGui::Separator();
		ImGui::BeginGroup();
		{
			if (ImGui::Button("Auto"))			
				this->currentSimulation->isAutomatic = !this->currentSimulation->isAutomatic;			
			ImGui::SameLine();

			// Take steps
			if (ImGui::Button("Step Once"))
				this->currentSimulation->Step(StepType::Once);
			ImGui::SameLine();

			if (ImGui::Button("Step Multiple"))
				this->currentSimulation->Step(StepType::Multiple);
			ImGui::SameLine();

			if (ImGui::Button("Reset"))
				this->currentSimulation->Reset();
		}
		ImGui::EndGroup();

		// Timeline
		if (this->currentSimulation->steps > 0)
		{
			if (ImGui::Button("Backward"))
				this->currentSimulation->Step(StepType::Backward);
			ImGui::SameLine();
			ImGui::SliderInt("", &this->currentSimulation->currentStep, min, max);
			ImGui::SameLine();
			if (ImGui::Button("Forward"))
				this->currentSimulation->Step(StepType::Forward);
		}
	}

	void GUI::DrawGrid()
	{
		ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
		ImGui::Begin("Example Grid");
		{
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			const int rows = 4;
			const int cols = 4;
			const ImVec2 screenSize = ImGui::GetWindowSize();
			const ImVec2 cellSize(screenSize.x / cols, screenSize.y / rows);
			static ImVec4 col(1.0f, 1.0f, 0.4f, 1.0f);
			const ImU32 col32 = ImColor(col);
			const float spacing = 2.0f;
			float sz = 64.0f;
			const ImVec2 p = ImGui::GetCursorScreenPos();

			float y = p.y + 4.0f;
			for (int i = 0; i < rows; ++i)
			{
				float x = p.x + 4.0f;
				for (int j = 0; j < cols; ++j)
				{
					draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz), col32);
					x += sz + spacing;
				}
				y += sz + spacing;
			}

			// Now use a dummy to separate
			ImGui::Dummy(ImVec2((sz + spacing) * cols, (sz + spacing * 2) * rows));
			ImGui::Separator();
			ImGui::Text("data here...");
		}
		ImGui::End();
	}

	void GUI::DrawTests()
	{
		// Draw any executed tests
		for (auto& test : this->tests)
		{
			if (test.enabled)
				DrawTest(test);
				//test.window.Draw(0);
		}
	}

	void GUI::DrawTest(Test & test)
	{
		ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin(test.name.c_str(), &test.enabled)) 
		{
			ImGui::End();
			return;
		}

		ImGui::TextUnformatted(test.output.c_str());
		ImGui::End();
	}

	void GUI::DrawSimulation()
	{
		// Draw the windows
		DrawWatcher();
		DrawVariableEditor();
		DrawOverlay();
		
		int bufferIndex = this->currentSimulation->currentStep;
		for (auto& window : this->currentSimulation->windows)
			window->Draw((unsigned)bufferIndex);

		static const std::string resultTitle = "Result";
		static bool askNext = false;
		NotificationWindow::Draw(this->showResult, resultTitle, this->currentSimulation->resultLog, askNext);
	}

	void GUI::DrawSTDOUT()
	{
		if (!settings.showSTDOUT)
			return;
	}

	void GUI::DrawOverlay()
	{
		static bool show = true;

		enum Corner { TopLeft, TopRight, BottomLeft, BottomRight };
		const float DISTANCE = 20.0f;
		static int corner = BottomLeft;
		ImVec2 window_pos = ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE : DISTANCE);
		ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
		if (ImGui::Begin("Controls", &show, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
		{
			DrawControls();
			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = TopLeft;
				if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = TopRight;
				if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = BottomLeft;
				if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = BottomRight;
				ImGui::EndPopup();
			}
			ImGui::End();
		}
		ImGui::PopStyleColor();

	}

	void GUI::DrawWatcher()
	{
		if (this->currentSimulation->watchVariables.empty() && this->currentSimulation->plotVariables.empty())
			return;

		//ImGui::SetNextWindowSize()
		ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);
		ImGui::Begin("Watcher");
		{
			// Watch      
			if (!this->currentSimulation->watchVariables.empty())
			{
				ImGui::Columns(2);

				// Header
				ImGui::TextColored(sf::Color::Red, "Name");
				ImGui::NextColumn();
				ImGui::TextColored(sf::Color::Yellow, "Value");
				ImGui::NextColumn();
				ImGui::Separator();

				for (auto& var : this->currentSimulation->watchVariables)
				{
					ImGui::Text(var->name.c_str());
					ImGui::NextColumn();
					ImGui::Text(var->lastValueToString.c_str());
					ImGui::NextColumn();
				}


				ImGui::Columns(1);
				ImGui::NewLine();
			}

			// Plots
			if (!this->currentSimulation->plotVariables.empty())
			{
				ImGui::Columns(3);
				// Header
				{
					ImGui::TextColored(sf::Color::Red, "Name");
					ImGui::NextColumn();
					ImGui::TextColored(sf::Color::Yellow, "Value");
					ImGui::NextColumn();
					ImGui::TextColored(sf::Color::Yellow, "Plot");
					ImGui::NextColumn();
					ImGui::Separator();
				}

				for (auto& var : this->currentSimulation->plotVariables)
				{
					// Name
					ImGui::Text(var->name.c_str());
					ImGui::NextColumn();
					// Value
					ImGui::Text(var->lastValueToString.c_str());
					ImGui::NextColumn();
					// Plot
					if (!var->history.empty())
						ImGui::PlotLines("", &var->history[0], static_cast<int>(var->history.size()));
					ImGui::NextColumn();

				}
			}
		}

		ImGui::Columns(1);
		ImGui::End();
	}

	void GUI::DrawVariableEditor()
	{
		if (!this->currentSimulation->HasEditableVariables())
			return;

		ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);
		ImGui::Begin("Variable Editor");
		{
			if (!this->currentSimulation->editableFloatVariables.empty())
			{
				for (auto& var : this->currentSimulation->editableFloatVariables)
				{
					if (var.slider)
						ImGui::SliderFloat(var.label.c_str(), var.value, var.min, var.max);
					else
						ImGui::InputFloat(var.label.c_str(), var.value);
				}
			}

			if (!this->currentSimulation->editableIntVariables.empty())
			{
				for (auto& var : this->currentSimulation->editableIntVariables)
				{
					if (var.slider)
						ImGui::SliderInt(var.label.c_str(), var.value, var.min, var.max);
					else
						ImGui::InputInt(var.label.c_str(), var.value);
				}
			}

			if (!this->currentSimulation->editableBoolVariables.empty())
			{
				for (auto& var : this->currentSimulation->editableBoolVariables)
				{
					ImGui::Checkbox(var.label.c_str(), var.value);
				}
			}
		}
		ImGui::End();
	}

}