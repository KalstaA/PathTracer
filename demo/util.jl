function random_point()
    return normalize([randn(), randn(), randn()])
end

function random_point_hemi(normal)
    random_dir = random_point()
    return random_dir * sign(dot(normal, random_dir))
end

function random_in_circle()
    angle = rand() * 2 * pi
    return [cos(angle), sin(angle)] * sqrt(rand())
end

function environment_light(ray)
    skycolor = [0.2, 0.5, 1.0]
    horizoncolor = [0.7, 0.8, 0.8]
    groundcolor = [0.1, 0.1, 0.1]
    
    if ray.direction[3] >= 0
        light = horizoncolor + ((skycolor - horizoncolor) .* abs(ray.direction[3]) ^ 0.8)
    else
        light = horizoncolor + ((groundcolor - horizoncolor) .* abs(ray.direction[3]) ^ 0.4)
    end

    return light
end

function reflect(vector, normal)
    return vector - 2 * dot(vector, normal) * normal
end

function create_ray(x, y)
    g_x = focusdistance * tan(camera_fov / 2);
    g_y = g_x * (res_y - 1) / (res_x - 1);
    
    q_x = 2 * g_x / (res_x - 1) * camera_right;
    q_y = 2 * g_y / (res_y - 1) * camera_up;
    
    jiggle = random_in_circle() * depth_of_field
    random = jiggle[1] * q_x + jiggle[2] * q_y
    origin = camera_pos + random

    bottom_left = camera_dir * focusdistance - g_x * camera_right - g_y * camera_up;
    jiggle = random_in_circle() * blur_radius
    random = jiggle[1] * q_x + jiggle[2] * q_y
    target = bottom_left + q_y * (res_y - y) + q_x * (x - 1) + random
    direction = normalize(-origin + target)

    return Ray(origin, direction)
end

function trace(ray::Ray)
    raycolor = [1.0, 1.0, 1.0]
    light = [0.0, 0.0, 0.0]

    for bounce in 1:max_bounces
        hit = ray_collision(ray)
        if hit.hit && hit.dist > 0.0001
            ray.origin = hit.point
            diffuse_direction = normalize(random_point() + hit.normal)
            specular_direction = reflect(normalize(ray.direction), hit.normal)

            ray.direction = diffuse_direction + (specular_direction - diffuse_direction) * hit.material.smoothness

            emitted_light = hit.material.emission_strength * hit.material.emission_color

            light += emitted_light .* raycolor
            raycolor .*= hit.material.color
        else
            light += environment_light(ray) .* raycolor
            break 
        end
    end

    return light;
end

function ray_collision(ray::Ray)

    closest_hit = Inf
    hit = Hit(false, Material([0, 0, 0], 0, [1, 1, 1], 0), [Inf, Inf, Inf], [Inf, Inf, Inf], Inf)
    
    for ball in balls
        a = dot(ray.direction, ray.direction)
        b = 2 * dot(ray.origin-ball.pos, ray.direction)
        c = dot(ray.origin-ball.pos, ray.origin-ball.pos) - ball.radius^2

        discriminant = b*b - 4*a*c
        distance = Inf
        if discriminant >= 0
            distance = (-b - sqrt(discriminant)) / (2*a)
        end

        if discriminant >= 0 && distance < closest_hit && distance > 0
            closest_hit = distance
            hit.material = ball.material
            hit.hit = true
            hit.point = ray.origin + ray.direction * distance
            hit.normal = normalize(hit.point - ball.pos)
        end
    end

    return hit
end

function renderframe()
    result = Array{Float64}(undef, 3, res_y, res_x);
    for y in 1:res_y, x in 1:res_x
        total_light = [0, 0, 0]
        for ray in 1:rays_per_pixel
            total_light += trace(create_ray(x, y))
        end
        [result[i, y, x] = total_light[i] / rays_per_pixel for i in 1:3]    
    end 
    return result
end

function clamp(input)
    return map(x -> (x > 1 ? 1.0 : x), input)
end