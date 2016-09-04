class ProcessingsController < ApplicationController
  before_action :set_processing, only: [:show, :edit, :update, :destroy]

  # GET /processings
  # GET /processings.json
  def index
    @processings = Processing.all
  end

  # GET /processings/1
  # GET /processings/1.json
  def show
  end

  # GET /processings/new
  def new
    @processing = Processing.new
  end

  # GET /processings/1/edit
  def edit
  end

  def save_processing
    Processing.create(processing_params)
  end

  # POST /processings
  # POST /processings.json
  def create
    @processing = Processing.new(processing_params)

    respond_to do |format|
      if @processing.save
        format.html { redirect_to @processing, notice: 'Processing was successfully created.' }
        format.json { render :show, status: :created, location: @processing }
      else
        format.html { render :new }
        format.json { render json: @processing.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /processings/1
  # PATCH/PUT /processings/1.json
  def update
    respond_to do |format|
      if @processing.update(processing_params)
        format.html { redirect_to @processing, notice: 'Processing was successfully updated.' }
        format.json { render :show, status: :ok, location: @processing }
      else
        format.html { render :edit }
        format.json { render json: @processing.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /processings/1
  # DELETE /processings/1.json
  def destroy
    @processing.destroy
    respond_to do |format|
      format.html { redirect_to processings_url, notice: 'Processing was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_processing
      @processing = Processing.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def processing_params
      params.require(:processing).permit(:name,:disp_enable, :var_enable, :median_enable, :wat_ime_enable, :wat_opencv_enable, :merge_enable, :dispT, :dispJ, :varT, :varJ, :medJ, :watT, :merT, :merJ)
    end
end
