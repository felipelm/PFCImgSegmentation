Rails.application.routes.draw do

  resources :commands
  resources :processings
  resources :images
  devise_for :users, :controllers => { registrations: 'users/registrations', sessions: "users/sessions", passwords: 'users/passwords' } do
    get "/", :to => "users/sessions#create"
  end

  get "send_parameters" => "images#processing"

  authenticated :user do
    root 'images#home'
  end

  unauthenticated :user do
    devise_scope :user do
      get "/" => "users/sessions#new"
    end
  end
end
