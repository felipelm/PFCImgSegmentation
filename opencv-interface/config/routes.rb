Rails.application.routes.draw do

  resources :processings
  resources :commands
  resources :images
  devise_for :users, :controllers => { registrations: 'users/registrations', sessions: "users/sessions", passwords: 'users/passwords' } do
    get "/", :to => "users/sessions#create"
  end

  get "send_parameters" => "images#processing"
  get "save_parameters" => "processings#create"
  get "choose_saved_processing" => "processings#choose_saved_processing"

  authenticated :user do
    root 'images#index'
  end

  unauthenticated :user do
    devise_scope :user do
      get "/" => "users/sessions#new"
    end
  end
end
