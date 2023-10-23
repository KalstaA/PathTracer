struct Material
    color::Vector{Float64}
    emission_strength::Float64
    emission_color::Vector{Float64}
    smoothness::Float64
end

struct Ball
    pos::Vector{Float64}
    radius::Float64
    material::Material
end

mutable struct Hit
    hit::Bool
    material::Material
    normal::Vector{Float64}
    point::Vector{Float64}
    dist::Float64
end

mutable struct Ray
    origin::Vector{Float64}
    direction::Vector{Float64}
end