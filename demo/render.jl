using Images
using LinearAlgebra

include("types.jl")
include("util.jl");

# Define render properties
res_x, res_y = 600, 500;

# Define scene info
camera_pos = [0, 0, 0];
camera_dir = [1, 0, 0];
camera_fov = pi / 3;
camera_up = [0, 0, 1];
camera_right = normalize(cross(camera_dir, camera_up));
focusdistance = 5;

blur_radius = 1; # anti-aliasing
depth_of_field = 50; 
max_bounces = 3;
rays_per_pixel = 20;

balls = Vector{Ball}(undef, 6);
balls[1] = Ball([5, 0, 0], 1, Material([1, 0, 0], 0, [1, 1, 1], 0.8));
balls[2] = Ball([4, -0.5, -0.5], 0.5, Material([0, 1, 0], 0, [1, 1, 1], 0));
balls[3] = Ball([6, 1.5, 1], 0.7, Material([1, 1, 1], 0, [1, 1, 1], 1));
balls[4] = Ball([6, 0, -31], 30, Material([0.5, 0.5, 0.5], 0, [1, 1, 1], 0));
balls[5] = Ball([10, -30, 20], 20, Material([1, 1, 0], 0.5, [1, 1, 1], 0));
balls[6] = Ball([6, 2, -0.5], 0.5, Material([0, 0, 1], 0, [1, 1, 1], 0));

# Render output image
output = zeros(3, res_y, res_x);
for sample in 0:10
    weight = 1.0 / (sample + 1)
    output = clamp(output * (1 - weight) + map(value -> sqrt(value), renderframe()) * weight)
    save("output.png", colorview(RGB, output))
end