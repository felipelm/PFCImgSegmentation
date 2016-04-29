class ImagesController < ApplicationController
  before_action :set_image, only: [:show, :edit, :update, :destroy]
  before_action :authenticate_user!
  require "opencv"

  # GET /images
  # GET /images.json
  def index
    @images = Image.all
  end

  # GET /images/1
  # GET /images/1.json
  def show
    if @image.image.to_s != ""
      path = "/Users/felipemachado/Documents/rails/projects/PFCImgSegmentation/opencv-interface/public"
      cvmat = OpenCV::CvMat.load(path+@image.image.to_s)
      cvmat = cvmat.BGR2GRAY
      canny = cvmat.canny(50, 150)
      contour = canny.find_contours(:mode => OpenCV::CV_RETR_LIST, :method => OpenCV::CV_CHAIN_APPROX_SIMPLE)
      while contour
        unless contour.hole?
          box = contour.bounding_rect
          cvmat.rectangle! box.top_left, box.bottom_right, :color => OpenCV::CvColor::Black
          box = contour.min_area_rect2
        end
        contour = contour.h_next
      end
      path2 = ""
      @image.image.to_s.split('/').each_with_index do |im, i|
        if i == @image.image.to_s.split('/').length-1
          path2 += "/canny_"+im
        elsif i != 0
          path2 += "/"+im
        end
      end
      broken_path = @image.image_url.to_s.split('/')
      broken_path[broken_path.size-1] = "canny_"+broken_path[broken_path.size-1]
      broken_path.join('/')
      @new_path = broken_path.join('/')
      cvmat.save_image(path+path2)
    end
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
end
