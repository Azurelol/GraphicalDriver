#pragma once

#include <string>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace Oxana
{
	struct Image
	{
		std::string name;
		std::string path;
		sf::Texture texture;
		bool loaded;

		Image(const std::string& name, const std::string& path, bool load = true) : name(name), path(path)
		{	
			if (load)
				this->Load();
		}

		void Load()
		{
			this->loaded = this->texture.loadFromFile(this->path);
			if (this->loaded)
			{
				
			}
		}

		
	};

}