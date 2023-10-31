#pragma once

class Renderer
{
private:

    Scene scene_;
    Camera camera_;

    int resolution_x;
    int resolution_y;

    float anti_alias_radius = 1;
    float depth_of_field = 10;

    int max_bounces = 3;
    int rays_per_pixel = 5;

    float view_width;
    float view_height;

    Vector topleft_pixel;
    Vector pixel_x;
    Vector pixel_y;

    Ray createRay(int x, int y) {

        // depth of field effect randomizes the origin
        Vector2 jiggle = randomInCircle() * depth_of_field;
        Vector randomShift = jiggle(0) * pixel_x + jiggle(1) * pixel_y;
        Point origin = camera_.position + randomShift;

        // anti-aliasing randomizes the target
        jiggle = randomInCircle() * anti_alias_radius;
        randomShift = jiggle(0) * pixel_x + jiggle(1) * pixel_y;
        Vector target = topleft_pixel + pixel_y * y + pixel_x * x + randomShift;

        Vector direction = (target - origin).normalized();

        return Ray{origin, direction, Color(1.0, 1.0, 1.0), Light(0.0, 0.0, 0.0)};
    }

    Hit rayCollision(Ray ray) {
        float closest_hit = INFINITY;

        Hit rayHit;
        rayHit.did_hit = false;

        for (auto ball : scene_.getBalls()) {
            Vector toBall = ray.origin - ball.getPosition();
            float a = ray.direction.dot(ray.direction);
            float b = 2 * ray.direction.dot(toBall);
            float c = toBall.dot(toBall) - ball.getRadius() * ball.getRadius();

            float discriminant = b*b - 4*a*c;
            float distance = INFINITY;

            if (discriminant >= 0)
            {
                distance = (-b - sqrt(discriminant)) / (2*a);
            }

            if (discriminant >= 0 && distance < closest_hit && distance > 0)
            {
                closest_hit = distance;
                rayHit.material = ball.getMaterial();
                rayHit.did_hit = true;
                rayHit.point = ray.origin + ray.direction * distance;
                rayHit.normal = (rayHit.point - ball.getPosition()).normalized();
            }
        }

        return rayHit;
    }

    Light trace(Ray ray) {
        for (int bounce = 0; bounce < max_bounces; bounce++)
        {
            Hit hit = rayCollision(ray);

            if (hit.did_hit && hit.distance > 0.0001) {
                ray.origin = hit.point;

                Vector diffuse_direction = (randomDirection() + hit.normal).normalized();
                // TO-DO specular reflection

                ray.direction = diffuse_direction;
                Light emitted_light = hit.material.emission_strength * hit.material.emission_color;
                
                ray.light += emitted_light.cwiseProduct(ray.color);
                ray.color = ray.color.cwiseProduct(hit.material.color);
            }

            else 
            {
                // TO-DO better environment light
                ray.light += Color(0.5, 0.5, 0.5).cwiseProduct(ray.color);
                break;
            }
        }
        return ray.light;
    }
    
public:

    Renderer(int res_x, int res_y, Scene sceneToRender) {
        resolution_x = res_x;
        resolution_y = res_y;
        scene_ = sceneToRender;
        camera_ = scene_.getCamera();
        view_width = tan(camera_.fov / 2);
        view_height = view_width * (resolution_y - 1) / (resolution_x - 1);
        pixel_x = 2 * view_width / (resolution_x - 1) * camera_.right;
        pixel_y = - 2 * view_height / (resolution_y - 1) * camera_.up;
        topleft_pixel = camera_.direction - view_width * camera_.right + view_height * camera_.up;
    }

    ~Renderer() = default;

    // TO-DO copy constructor and copy assigment

    /*
    * Returns a vector of vectors containing the colors of each pixel in the output image
    */
    auto render() {
        
        std::vector<std::vector<Color>> result(resolution_x, std::vector<Color> (resolution_y));
        
        for (int x = 0; x < resolution_x; x++)
        {
            for (int y = 0; y < resolution_y; y++)
            {
                Light totalLight(0, 0, 0);

                for (int rayNum = 0; rayNum < rays_per_pixel; rayNum++)
                {
                    totalLight += trace(createRay(x, y));
                }
                result[x][y] = clamp(totalLight / rays_per_pixel);
            }   
        }

        return result;
    }
};
