#include <components/TilesRenderer.hpp>

#include <components/Collider.hpp>

#include <SFML/Graphics/Sprite.hpp>

#include <tools/pugixml.hpp>
#include <iostream>
#include <string>

TilesRenderer::TilesRenderer(sf::RenderWindow& window) : Component("Tiles Renderer"), window(window) {
}

TilesRenderer::~TilesRenderer() {
    for(auto& texture : textures) {
        delete texture;
        texture = nullptr;
    }
}

void TilesRenderer::init() {

    // map file
    const std::string map_filename = "map3.xml";
    pugi::xml_document map_document;
    if (!map_document.load_file(map_filename.c_str())) {
        std::cerr << "Erreur : Impossible de charger le fichier XML\n";
        return;
    }

    const pugi::xml_node& map = map_document.child("map");

    width = map.attribute("width").as_int();
    height = map.attribute("height").as_int();
    tile_width = map.attribute("tilewidth").as_int();
    tile_height = map.attribute("tileheight").as_int();

    // for each tileset
    for(pugi::xml_node tileset : map.children("tileset")) {
        const int first_id = tileset.attribute("firstgid").as_int();
        std::string source_path = tileset.attribute("source").value();

        // tileset file
        pugi::xml_document tileset_document;
        if (!tileset_document.load_file(source_path.c_str())) {
            std::cerr << "Erreur : Impossible de charger le fichier XML2\n";
            return;
        }

        int last_id = first_id + tileset_document.child("tileset").attribute("tilecount").as_int() - 1;
        std::string texture_path = tileset_document.child("tileset").child("image").attribute("source").value();

        sf::Texture* texture = new sf::Texture();
        if(!texture->loadFromFile(texture_path)) {
            std::cout << "Erreur lors du chargement de la texture " << texture_path << std::endl;
        }
        else {
            first_tile_ids.push_back(first_id);
            last_tile_ids.push_back(last_id);
            textures.push_back(texture);
        }
    }

    // for each layer
    for(pugi::xml_node layer : map.children("layer")) {
        // splits the content into vector of codes
        std::vector<int> tiles_codes;
        std::string code;
        std::string content = layer.child("data").text().as_string();
        std::stringstream ss(content);
        while(getline(ss, code, ',')) {
            tiles_codes.push_back(std::stoi(code));
        }

        // puts (x, y, layer id, tile code) in layer_vector if tile code != 0
        std::vector<std::tuple<int,int,int,int>> layer_vector;
        for(int y = 0; y < height; y++) {
            for(int x = 0; x < width; x++) {
                int i = y * width + x;
                int val = tiles_codes[i];
                if(val == 0) continue;
                sf::Vector2i pos = {x,y};
                int texture_id;
                for(int t=0; t<first_tile_ids.size(); t++) {
                    if(t == first_tile_ids.size()-1) texture_id = t;
                    if(val >= first_tile_ids[t] && val <= last_tile_ids[t]) {
                        texture_id = t;
                        break;
                    }
                }
                layer_vector.push_back(std::make_tuple(x,y,texture_id, val-first_tile_ids[texture_id]));
            }
        }
        // std::cout << layer_vector.size() << std::endl;
        tiles.push_back(layer_vector);
    }

    // for each object
    for(pugi::xml_node object : map.child("objectgroup").children("object")) {
        int w = object.attribute("width").as_int();
        int h = object.attribute("height").as_int();
        float x = object.attribute("x").as_float() + (w/2);
        float y = object.attribute("y").as_float() + (h/2);
        Collider* collider = new Collider(w, h, false, {x, y});
        getParent().addComponent(*collider);
    }

    // std::cout << "width : " << width << " | height : " << height << std::endl;
    // std::cout << "tilewidth : " << tile_width << " | tileheight : " << tile_height << std::endl;
    // for(auto& layer : tiles) {
    //     for(auto& tile : layer) {
    //         std::cout << std::get<0>(tile) << " | " << std::get<1>(tile) << " | " << std::get<2>(tile) << " | " << std::get<3>(tile) << std::endl;
    //     }
    // }
}

void TilesRenderer::update() {
    for(auto& layer : tiles)
    for(auto& tile : layer) {
        int x = std::get<0>(tile);
        int y = std::get<1>(tile);
        int text = std::get<2>(tile);
        int code = std::get<3>(tile);
        sf::Sprite sprite(*textures[text]);
        int max_i = (textures[text]->getSize().x / tile_width);
        int i = code % max_i;
        int j = (int) (code / max_i);
        sprite.setTextureRect(sf::IntRect({i * tile_width, j * tile_height}, {tile_width, tile_height}));
        sprite.setPosition({(float)(x * tile_width), (float)(y * tile_height)});
        window.draw(sprite);
    }
}