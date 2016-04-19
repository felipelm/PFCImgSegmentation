Rails.application.routes.draw do

  resources :images
  devise_for :users, controllers: {
        sessions: 'users/sessions'
      }

  authenticated :user do
    root 'images#index'
  end

  # unauthenticated :users do
  #   devise_scope :users do
  #     get "/" => "users/sessions#new"
  #   end
  # end
end
