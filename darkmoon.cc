--// Settings
local FOV_RADIUS = 100 -- Size of the FOV circle
local ENABLED = false -- Default state of aim assist
local AIM_PART = "Head" -- Target part (Head, Torso, etc.)

--// Services
local Players = game:GetService("Players")
local UserInputService = game:GetService("UserInputService")
local RunService = game:GetService("RunService")
local Camera = workspace.CurrentCamera
local LocalPlayer = Players.LocalPlayer

--// UI Elements
local ScreenGui = Instance.new("ScreenGui", game.CoreGui)
local ToggleButton = Instance.new("TextButton", ScreenGui)
local Notification = Instance.new("TextLabel", ScreenGui)
local FOVCircle = Instance.new("Frame", ScreenGui)

-- UI Setup
ToggleButton.Size = UDim2.new(0, 100, 0, 50)
ToggleButton.Position = UDim2.new(0.05, 0, 0.8, 0)
ToggleButton.Text = "Aim Assist: OFF"
ToggleButton.BackgroundColor3 = Color3.fromRGB(255, 0, 0)

Notification.Size = UDim2.new(0, 150, 0, 50)
Notification.Position = UDim2.new(0.4, 0, 0.1, 0)
Notification.Text = ""
Notification.Visible = false

FOVCircle.Size = UDim2.new(0, FOV_RADIUS * 2, 0, FOV_RADIUS * 2)
FOVCircle.Position = UDim2.new(0.5, -FOV_RADIUS, 0.5, -FOV_RADIUS)
FOVCircle.BackgroundColor3 = Color3.fromRGB(255, 255, 255)
FOVCircle.BackgroundTransparency = 0.5
FOVCircle.Visible = true
FOVCircle.BorderSizePixel = 2

--// Toggle Function
ToggleButton.MouseButton1Click:Connect(function()
    ENABLED = not ENABLED
    ToggleButton.Text = ENABLED and "Aim Assist: ON" or "Aim Assist: OFF"
    ToggleButton.BackgroundColor3 = ENABLED and Color3.fromRGB(0, 255, 0) or Color3.fromRGB(255, 0, 0)
    Notification.Text = ENABLED and "Aim Assist Enabled" or "Aim Assist Disabled"
    Notification.Visible = true
    wait(1.5)
    Notification.Visible = false
end)

--// Bullet Lock Function
local function getNearestTarget()
    local closestEnemy = nil
    local shortestDistance = FOV_RADIUS
    
    for _, player in pairs(Players:GetPlayers()) do
        if player ~= LocalPlayer and player.Character and player.Character:FindFirstChild(AIM_PART) then
            local targetPos = Camera:WorldToViewportPoint(player.Character[AIM_PART].Position)
            local mousePos = UserInputService:GetMouseLocation()
            local distance = (Vector2.new(targetPos.X, targetPos.Y) - mousePos).Magnitude
            
            if distance < shortestDistance then
                shortestDistance = distance
                closestEnemy = player.Character[AIM_PART]
            end
        end
    end
    
    return closestEnemy
end

local function onBulletFired(bullet)
    if ENABLED then
        local target = getNearestTarget()
        if target then
            bullet.CFrame = CFrame.new(bullet.Position, target.Position)
        end
    end
end

--// Hook into bullet creation (Assumes bullets are projectiles in workspace)
workspace.ChildAdded:Connect(function(child)
    if child:IsA("Part") and child.Name == "Bullet" then -- Adjust name if necessary
        onBulletFired(child)
    end
end)
