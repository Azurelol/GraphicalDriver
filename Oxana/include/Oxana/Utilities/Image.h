#pragma once

#include <string>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Vector.h"

namespace Oxana
{
	// Class for basic image manipulation
	class Image
	{
		sf::Image image;
		Vector2 size;
		sf::Color fillColor;

		public:
		Image(float width, float height, sf::Color color = sf::Color::White)
		{
			this->Reset(width, height, color);
		}

		void Save(const std::string& path)
		{
			image.saveToFile(path.c_str());
		}

		void Reset(float width, float height, sf::Color fillColor = sf::Color::White)
		{
			this->size = Vector2(width, height);
			this->fillColor = fillColor;
			this->Reset();
		}

		void Reset()
		{		
			
			image.create(this->size.x, this->size.y, this->fillColor);
		}

		void SetPixel(int x, int y, sf::Color color)
		{
			image.setPixel(x, y, color);
		}

		void FlipVertically()
		{
			image.flipVertically();
		}

		void FlipHorizontally()
		{
			image.flipHorizontally();
		}

		sf::Texture ToTexture() const
		{
			sf::Texture texture;
			texture.loadFromImage(this->image);
			return texture;
		}

	};

	// Class used for loading an iamge into a texture to be drawn
	struct Texture
	{
		std::string name;
		std::string path;
		sf::Texture texture;
		bool loaded;

		Texture(const sf::Image& image)
		{			
			this->loaded = this->texture.loadFromImage(image);
		}

		Texture(const sf::Texture& texture)
		{
			this->texture = texture;
			this->loaded = true;
		}

		Texture(const std::string& name, const std::string& path, bool load = true) : name(name), path(path)
		{	
			if (load)
				this->Load();
		}

		void Load()
		{
			this->loaded = this->texture.loadFromFile(this->path);
		}		
	};

}