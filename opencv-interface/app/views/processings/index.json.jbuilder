json.array!(@processings) do |processing|
  json.extract! processing, :id, :disp_enable, :var_enable, :median_enable, :wat_ime_enable, :wat_opencv_enable, :merge_enable, :dispT, :dispJ, :varT, :varJ, :medJ, :watT, :merT, :merJ
  json.url processing_url(processing, format: :json)
end
