class AddUserToProcessings < ActiveRecord::Migration
  def change
    add_reference :processings, :user, index: true, foreign_key: true
  end
end
