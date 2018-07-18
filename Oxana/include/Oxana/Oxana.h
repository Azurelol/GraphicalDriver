#pragma once

// Include
#include "Include.h"
#include "UnitTest.h"
#include "Simulation.h"

namespace Oxana
{
	class GUI
	{
	public:

		// Default settings for the window rendering
		struct Settings
		{
			// Title of the window
			std::string title;
			// At what framerate to run this window
			unsigned framerate;
			// Width of the window
			int width;
			// Height of the window
			int height;
			// The background color
			sf::Color background;
			bool showSTDOUT;
			std::string stdoutCaptureFile;
			bool showOverlay;
			DriverFunction originalDriver;
			Settings();
		};

		Settings settings;

		// ------------------ Methods ----------------/
		// Constructor that takes a settings object
		GUI(const Settings& settings);
		// Adds a simuilation to the GUI
		void Add(Simulation simulation);
		// Adds an unit test to the GUI
		void Add(TestSuite test); 
		// Starts running the GUI driver
		void Run();

	private:
		//---------------------------------------------------------------------------/
		// Fields
		//---------------------------------------------------------------------------/
		std::vector<Simulation> simulations;
		std::vector<TestSuite> tests;
		Simulation* currentSimulation;
		std::unique_ptr<sf::RenderWindow> window;
		FILE* outputFile;
		std::stringstream outputStream;
		ImGuiTextBuffer log;
		bool showResult;
		bool hasTests, hasSimulations;

		// ------------------ Methods ----------------/
		void Update();
		void Draw();
		void OnInput(sf::Keyboard::Key key);

		void DrawMenu();
		void DrawControls();
		void DrawGrid();
		void DrawTests();
		void DrawTest(TestSuite& test);
		void DrawSimulation();
		void DrawWatcher();
		void DrawVariableEditor();
		void DrawSTDOUT();
		void DrawOverlay();

		void Initialize();
		void Shutdown();
		void Assert();
		void Set(Simulation* simulation);
		void Run(TestSuite* test);
	};

}