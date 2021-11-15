#include "ClientNetwork.hpp"
#include "CloseHandler.hpp"
#include "ModulesManager.hpp"


namespace Distance
{
	namespace _internal
	{
		Distance::Network::DefaultTcpClient* gClient{};
		bool* gIsRunClient{};

		LRESULT HandlerRoutine(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
		{
			switch (uMessage)
			{
			case WM_QUERYENDSESSION:
			case WM_CLOSE:
				DestroyWindow(hWindow);
				break;
			case WM_DESTROY:
				*gIsRunClient = false;
				gClient->stop();
				ModulesManager::invoke().stopAll();
				PostQuitMessage(EXIT_SUCCESS);
				break;
			default:
				return DefWindowProc(hWindow, uMessage, wParam, lParam);
			}
			return EXIT_SUCCESS;
		}
	}

	void CloseHandler(
		std::shared_ptr<Distance::Network::DefaultTcpClient> client, 
		std::shared_ptr<bool> isRunClient
	)
	{
		_internal::gClient = client.get();
		_internal::gIsRunClient = isRunClient.get();

		MSG msg{};
		WNDCLASSEX wnd_MainWindowClient{};
		wnd_MainWindowClient.cbSize = sizeof(WNDCLASSEX);
		wnd_MainWindowClient.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wnd_MainWindowClient.hIcon = nullptr;
		wnd_MainWindowClient.hIconSm = nullptr;
		wnd_MainWindowClient.style = CS_HREDRAW | CS_VREDRAW;
		wnd_MainWindowClient.lpfnWndProc = (WNDPROC)_internal::HandlerRoutine;
		wnd_MainWindowClient.hInstance = GetModuleHandle(nullptr);
		wnd_MainWindowClient.lpszClassName = $T"DistanceClientMainWindow";
		if (!RegisterClassEx(&wnd_MainWindowClient))
		{
			throw std::runtime_error("Failed to register the class DistanceClientMainWindow!");
		}

		auto MainWindowClient = CreateWindow(
			$T"DistanceClientMainWindow",
			nullptr,
			WS_OVERLAPPED,
			0,
			0,
			0,
			0,
			nullptr,
			nullptr,
			wnd_MainWindowClient.hInstance,
			nullptr
		);
		if (MainWindowClient == INVALID_HANDLE_VALUE)
		{
			throw std::runtime_error("Failed to create the DistanceClientMainWindow!");
		}

		ShowWindow(MainWindowClient, SW_HIDE);
		UpdateWindow(MainWindowClient);

		while (GetMessage(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		UnregisterClass($T"DistanceClientMainWindow", wnd_MainWindowClient.hInstance);
	}
}