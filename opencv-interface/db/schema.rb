# encoding: UTF-8
# This file is auto-generated from the current state of the database. Instead
# of editing this file, please use the migrations feature of Active Record to
# incrementally modify your database, and then regenerate this schema definition.
#
# Note that this schema.rb definition is the authoritative source for your
# database schema. If you need to create the application database on another
# system, you should be using db:schema:load, not running all the migrations
# from scratch. The latter is a flawed and unsustainable approach (the more migrations
# you'll amass, the slower it'll run and the greater likelihood for issues).
#
# It's strongly recommended that you check this file into your version control system.

ActiveRecord::Schema.define(version: 20160909163450) do

  create_table "commands", force: :cascade do |t|
    t.string   "cmd"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
  end

  create_table "images", force: :cascade do |t|
    t.string   "image_path"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
    t.string   "image"
    t.integer  "user_id"
  end

  add_index "images", ["user_id"], name: "index_images_on_user_id"

  create_table "processings", force: :cascade do |t|
    t.boolean  "disp_enable"
    t.boolean  "var_enable"
    t.boolean  "median_enable"
    t.boolean  "wat_ime_enable"
    t.boolean  "wat_opencv_enable"
    t.boolean  "merge_enable"
    t.integer  "dispT"
    t.integer  "dispJ"
    t.integer  "varT"
    t.integer  "varJ"
    t.integer  "medJ"
    t.integer  "watT"
    t.integer  "merT"
    t.integer  "merJ"
    t.datetime "created_at",        null: false
    t.datetime "updated_at",        null: false
    t.string   "name"
    t.integer  "user_id"
  end

  add_index "processings", ["user_id"], name: "index_processings_on_user_id"

  create_table "users", force: :cascade do |t|
    t.string   "email",                  default: "", null: false
    t.string   "encrypted_password",     default: "", null: false
    t.string   "reset_password_token"
    t.datetime "reset_password_sent_at"
    t.datetime "remember_created_at"
    t.integer  "sign_in_count",          default: 0,  null: false
    t.datetime "current_sign_in_at"
    t.datetime "last_sign_in_at"
    t.string   "current_sign_in_ip"
    t.string   "last_sign_in_ip"
    t.datetime "created_at",                          null: false
    t.datetime "updated_at",                          null: false
  end

  add_index "users", ["email"], name: "index_users_on_email", unique: true
  add_index "users", ["reset_password_token"], name: "index_users_on_reset_password_token", unique: true

end
