class ImagesController < ApplicationController
  before_action :set_image, only: [:show, :edit, :update, :destroy]
  before_action :authenticate_user!

  # GET /images
  # GET /images.json
  def index
    @images = Image.all
  end

  def home

  end

  # GET /images/1
  # GET /images/1.json
  def show
    @processings = Processing.all
  end

  def processing
    @image = Image.find(params[:imgId])
    pre_processing(params)
    watershed_processing(params) if has_watershed_ime(params) && !has_post_processing(params)
    watershed_opencv_processing(params) if has_watershed_opencv(params) && !has_post_processing(params)
    post_processing(params) if has_post_processing(params)
    head :no_content
  end

  # GET /images/new
  def new
    @image = Image.new
  end

  # GET /images/1/edit
  def edit
  end

  # POST /images
  # POST /images.json
  def create
    @image = Image.new(image_params)
    @image.user_id = current_user.id

    respond_to do |format|
      if @image.save
        format.html { redirect_to @image, notice: 'Image was successfully created.' }
        format.json { render :show, status: :created, location: @image }
      else
        format.html { render :new }
        format.json { render json: @image.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /images/1
  # PATCH/PUT /images/1.json
  def update
    respond_to do |format|
      if @image.update(image_params)
        format.html { redirect_to @image, notice: 'Image was successfully updated.' }
        format.json { render :show, status: :ok, location: @image }
      else
        format.html { render :edit }
        format.json { render json: @image.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /images/1
  # DELETE /images/1.json
  def destroy
    @image.destroy
    respond_to do |format|
      format.html { redirect_to images_url, notice: 'Image was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_image
      @image = Image.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def image_params
      params.require(:image).permit(:image_path, :image)
    end

    def has_pre_processing(params)
      return params[:dispersion] == "true" || params[:variance] == "true" || params[:median] == "true"
    end

    def has_watershed_opencv(params)
      return params[:watershed_opencv] == "true"
    end

    def has_watershed_ime(params)
      return params[:watershed_ime] == "true"
    end

    def has_post_processing(params)
      return params[:merge] == "true"
    end

    def pre_processing(params)
      if params[:dispersion] == "true"
        dispersion(params)
      elsif params[:variance] == "true"
        variance(params)
      else
        copy_original
      end

      if params[:median] == "true"
        median(params)
      end
    end

    def copy_original
      original_image = `pwd`.chomp+"/public"+@image.image_url
      destination_image = `pwd`.chomp+"/public"+File.dirname(@image.image_url)+"/"+File.basename(@image.image_url,".*")+"_pre.jpg"
      result = "cp "+original_image+" "+destination_image
      `#{result}`
    end

    def dispersion(params)
      exec_path = "./vendor/assets/opencv/Dispersion"
      original_image = " -o "+`pwd`.chomp+"/public"+@image.image_url
      destination_image = " -d "+`pwd`.chomp+"/public"+File.dirname(@image.image_url)+"/"+File.basename(@image.image_url,".*")+"_pre.jpg"
      threshold = " -t "+params[:dispT]
      window_size = " -f "+params[:dispJ]
      result = exec_path + original_image + destination_image + threshold + window_size
      `#{result}`
    end

    def variance(params)
      exec_path = "./vendor/assets/opencv/Variance"
      original_image = " -o "+`pwd`.chomp+"/public"+@image.image_url
      destination_image = " -d "+`pwd`.chomp+"/public"+File.dirname(@image.image_url)+"/"+File.basename(@image.image_url,".*")+"_pre.jpg"
      threshold = " -t "+params[:varT]
      window_size = " -f "+params[:varJ]
      result = exec_path + original_image + destination_image + threshold + window_size
      `#{result}`
    end

    def median(params)
      exec_path = "./vendor/assets/opencv/Median"
      original_image = " -o "+`pwd`.chomp+"/public"+File.dirname(@image.image_url)+"/"+File.basename(@image.image_url,".*")+"_pre.jpg"
      destination_image = " -d "+`pwd`.chomp+"/public"+File.dirname(@image.image_url)+"/"+File.basename(@image.image_url,".*")+"_pre.jpg"
      filter_size = " -f "+params[:medJ]
      result = exec_path + original_image + destination_image + filter_size
      `#{result}`
    end

    def watershed_processing(params)
      exec_path = "./vendor/assets/opencv/Watershed"
      original_image = " -o "+`pwd`.chomp+"/public"+File.dirname(@image.image_url)+"/"+File.basename(@image.image_url,".*")+"_pre.jpg"
      destination_image = " -d "+`pwd`.chomp+"/public"+File.dirname(@image.image_url)+"/"+File.basename(@image.image_url,".*")+"_post.jpg"
      filter_size = " -f 1"
      result = exec_path + original_image + destination_image + filter_size
      `#{result}`
    end

    def color_image(params)
      exec_path = "./vendor/assets/opencv/Color"
      original_image = " -o "+`pwd`.chomp+"/public"+File.dirname(@image.image_url)+"/"+File.basename(@image.image_url,".*")+"_post.jpg"
      destination_image = " -d "+`pwd`.chomp+"/public"+File.dirname(@image.image_url)+"/"+File.basename(@image.image_url,".*")+"_post.jpg"
      result = exec_path + original_image + destination_image
      `#{result}`
    end

    def watershed_opencv_processing(params)
      exec_path = "./vendor/assets/opencv/WatershedOpenCV"
      original_image = " -o "+`pwd`.chomp+"/public"+File.dirname(@image.image_url)+"/"+File.basename(@image.image_url,".*")+"_pre.jpg"
      destination_image = " -d "+`pwd`.chomp+"/public"+File.dirname(@image.image_url)+"/"+File.basename(@image.image_url,".*")+"_post.jpg"
      result = exec_path + original_image + destination_image
      `#{result}`
    end

    def post_processing(params)
      exec_path = "./vendor/assets/opencv/MergeWatershed"
      original_image = " -o "+`pwd`.chomp+"/public"+@image.image_url
      destination_image = " -d "+`pwd`.chomp+"/public"+File.dirname(@image.image_url)+"/"+File.basename(@image.image_url,".*")+"_post.jpg"
      threshold_merge = " -t "+params[:merT]
      repeat = " -r "+params[:merR]
      threshold_disp = " -tD "+params[:dispT]
      window_size_disp = " -fD "+params[:dispJ]

      if params[:median] == "true"
        filter_size_median = " -fM "+params[:medJ]
      else
        filter_size_median = " -fM 0"
      end
      result = exec_path + original_image + destination_image + threshold_merge + repeat +
        threshold_disp + window_size_disp + filter_size_median
      `#{result}`
    end
end
