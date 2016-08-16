Rails.application.routes.draw do

  resources :commands
  resources :processings
  resources :images
  devise_for :users, :controllers => { registrations: 'users/registrations', sessions: "users/sessions", passwords: 'users/passwords' } do
    get "/", :to => "users/sessions#create"
  end

  authenticated :user do
    root 'images#index'
  end

  unauthenticated :user do
    devise_scope :user do
      get "/" => "users/sessions#new"
    end
  end
end
