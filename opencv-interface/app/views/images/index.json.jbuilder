json.array!(@images) do |image|
  json.extract! image, :id, :image_path
  json.url image_url(image, format: :json)
end
