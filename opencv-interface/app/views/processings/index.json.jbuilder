json.array!(@processings) do |processing|
  json.extract! processing, :id, :image_id, :user_id
  json.url processing_url(processing, format: :json)
end
