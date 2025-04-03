local MarketplaceService = game:GetService("MarketplaceService")
local Players = game:GetService("Players")
local UserInputService = game:GetService("UserInputService")

local GamePassID = 12345678 -- Replace with your actual Game Pass ID
local player = Players.LocalPlayer
local aimAssistEnabled = true -- Default to enabled

-- 🟢 Function to check if player owns the GamePass
local function hasGamePass()
    return MarketplaceService:UserOwnsGamePassAsync(player.UserId, GamePassID)
end

-- 🎯 Function to find closest enemy
local function getClosestEnemy()
    local character = player.Character
    if not character then return nil end

    local humanoidRootPart = character:FindFirstChild("HumanoidRootPart")
    if not humanoidRootPart then return nil end

    local closestEnemy = nil
    local shortestDistance = math.huge

    for _, otherPlayer in pairs(Players:GetPlayers()) do
        if otherPlayer ~= player and otherPlayer.Character then
            local enemyRootPart = otherPlayer.Character:FindFirstChild("HumanoidRootPart")
            if enemyRootPart then
                local distance = (humanoidRootPart.Position - enemyRootPart.Position).magnitude
                if distance < shortestDistance then
                    shortestDistance = distance
                    closestEnemy = enemyRootPart
                end
            end
        end
    end

    return closestEnemy
end

-- 🔫 Function to assist aim
local function aimAssist(bulletDirection)
    if hasGamePass() and aimAssistEnabled then
        local enemy = getClosestEnemy()
        if enemy then
            -- Adjust bullet direction slightly towards the enemy
            return (enemy.Position - player.Character.Head.Position).unit
        end
    end
    return bulletDirection
end

-- 🖥️ Create AI-Designed UI
local ScreenGui = Instance.new("ScreenGui")
ScreenGui.Parent = game.Players.LocalPlayer:WaitForChild("PlayerGui")

local Frame = Instance.new("Frame")
Frame.Size = UDim2.new(0.2, 0, 0.1, 0)
Frame.Position = UDim2.new(0.4, 0, 0.85, 0)
Frame.BackgroundColor3 = Color3.fromRGB(25, 25, 25)
Frame.BorderSizePixel = 2
Frame.Parent = ScreenGui

local ToggleButton = Instance.new("TextButton")
ToggleButton.Size = UDim2.new(1, 0, 1, 0)
ToggleButton.Text = "Aim Assist: ON"
ToggleButton.BackgroundColor3 = Color3.fromRGB(0, 200, 0)
ToggleButton.TextScaled = true
ToggleButton.Parent = Frame

-- 🎛️ Button Functionality
ToggleButton.MouseButton1Click:Connect(function()
    aimAssistEnabled = not aimAssistEnabled
    ToggleButton.Text = "Aim Assist: " .. (aimAssistEnabled and "ON" or "OFF")
    ToggleButton.BackgroundColor3 = aimAssistEnabled and Color3.fromRGB(0, 200, 0) or Color3.fromRGB(200, 0, 0)
end)

-- 📌 Hook into your gun system (Modify this to match your shooting script)
game:GetService("RunService").RenderStepped:Connect(function()
    if UserInputService:IsMouseButtonPressed(Enum.UserInputType.MouseButton1) then
        local bulletDirection = game.Players.LocalPlayer:GetMouse().Hit.LookVector
        bulletDirection = aimAssist(bulletDirection) -- Apply aim assist
        -- Fire the bullet with adjusted direction (modify your shooting script accordingly)
    end
end)
