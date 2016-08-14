require 'test_helper'

class ProcessingsControllerTest < ActionController::TestCase
  setup do
    @processing = processings(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:processings)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create processing" do
    assert_difference('Processing.count') do
      post :create, processing: { image_id: @processing.image_id, user_id: @processing.user_id }
    end

    assert_redirected_to processing_path(assigns(:processing))
  end

  test "should show processing" do
    get :show, id: @processing
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @processing
    assert_response :success
  end

  test "should update processing" do
    patch :update, id: @processing, processing: { image_id: @processing.image_id, user_id: @processing.user_id }
    assert_redirected_to processing_path(assigns(:processing))
  end

  test "should destroy processing" do
    assert_difference('Processing.count', -1) do
      delete :destroy, id: @processing
    end

    assert_redirected_to processings_path
  end
end
