#include "threepp/threepp.hpp"
#include <vector>
#include <memory>
#include <cmath>

using namespace threepp;

int main() {
    Canvas canvas("threepp - Sinus og fargeskifte");
    GLRenderer renderer(canvas);

    auto scene = Scene::create();
    scene->background = Color::aliceblue;

    auto camera = PerspectiveCamera::create(
        60, canvas.aspect(), 0.1f, 1000
    );
    camera->position.z = 10;

    canvas.onWindowResize([&](WindowSize size) {
        camera->aspect = size.aspect();
        camera->updateProjectionMatrix();
        renderer.setSize(size);
    });

    auto light = DirectionalLight::create();
    light->position.set(1, 2, 3);
    scene->add(light);

    scene->add(AmbientLight::create(0xffffff, 0.4f));

    std::vector<std::shared_ptr<Mesh>> kuber;
    std::vector<float> startY;

    auto geometry = BoxGeometry::create(1, 1, 1);

    // Hver rad trenger sitt eget materiale.
    auto materialeRad1 = MeshStandardMaterial::create();
    materialeRad1->color = Color::blue;

    auto materialeRad2 = MeshStandardMaterial::create();
    materialeRad2->color = Color::blue;

    for (int rad = 0; rad < 2; rad++) {
        for (int kolonne = 0; kolonne < 4; kolonne++) {
            auto material = materialeRad1;

            if (rad == 1) {
                material = materialeRad2;
            }

            auto box = Mesh::create(geometry, material);

            box->position.x = kolonne * 2.0f - 3.0f;
            box->position.y = 1.5f - rad * 3.0f;

            scene->add(box);
            kuber.push_back(box);
            startY.push_back(box->position.y);
        }
    }

    int fargeSteg = 0;

    canvas.onKeyPressed([&](KeyEvent event) {
        if (event.key == Key::SPACE) {
            fargeSteg++;

            if (fargeSteg == 1) {
                materialeRad1->color = Color::orange;
            }
            else if (fargeSteg == 2) {
                materialeRad2->color = Color::orange;
            }
            else {
                materialeRad1->color = Color::blue;
                materialeRad2->color = Color::blue;
                fargeSteg = 0;
            }
        }
    });

    const float pi = 3.14159265f;
    float A = 0.5f;
    float f = 0.5f;

    Clock clock;
    float tid = 0;

    canvas.animate([&] {
        const float dt = clock.getDelta();
        tid += dt;

        for (int i = 0; i < 8; i++) {
            float fase = 0;

            if (i % 2 == 1) {
                fase = pi;
            }

            // Sinusbevegelse opp og ned.
            kuber[i]->position.y =
                startY[i] + A * std::sin(2 * pi * f * tid + fase);

            // Rotasjon rundt kubens egen y-akse.
            kuber[i]->rotation.y += 1.0f * dt;
        }

        renderer.render(*scene, *camera);
    });
}