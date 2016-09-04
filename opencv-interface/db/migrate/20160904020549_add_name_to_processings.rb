class AddNameToProcessings < ActiveRecord::Migration
  def change
    add_column :processings, :name, :string
  end
end
